#include <par.h>
#include <parwin.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_NORMAL, "a_normal")     \
    F(U_MVP, "u_mvp")           \
    F(U_IMV, "u_imv")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

typedef struct {
    par_buffer* coords;
    par_buffer* normals;
    par_buffer* indices;
    int slices;
    int stacks;
    int ntriangles;
} Surface;

Matrix4 projection;
Matrix4 model;
Matrix4 view;
Surface knot_surface;

#define TWOPI 6.28318530718

Point3 knot_fn(float s, float t)
{
    const float a = 0.5f; const float b = 0.3f;
    const float c = 0.5f; const float d = 0.1f;
    const float u = (1 - s) * 2 * TWOPI;
    const float v = t * TWOPI;
    const float r = a + b * cos(1.5f * u);
    const float x = r * cos(u);
    const float y = r * sin(u);
    const float z = c * sin(1.5f * u);

    Vector3 dv;
    dv.x = -1.5f * b * sin(1.5f * u) * cos(u) - (a + b * cos(1.5f * u)) * sin(u);
    dv.y = -1.5f * b * sin(1.5f * u) * sin(u) + (a + b * cos(1.5f * u)) * cos(u);
    dv.z = 1.5f * c * cos(1.5f * u);

    Vector3 q = V3Normalize(dv);
    Vector3 qvn = V3Normalize((Vector3){q.y, -q.x, 0});
    Vector3 ww = V3Cross(q, qvn);

    Point3 range;
    range.x = x + d * (qvn.x * cos(v) + ww.x * sin(v));
    range.y = y + d * (qvn.y * cos(v) + ww.y * sin(v));
    range.z = z + d * ww.z * sin(v);
    return range;
}

void create_torus(float major, float minor, Surface* surf)
{
    int slices = surf->slices;
    int stacks = surf->stacks;
    float ds = 1.0f / slices;
    float dt = 1.0f / stacks;
    int vertexCount = slices * stacks * 3;
    int vertexStride = sizeof(float) * 3;
    surf->coords = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    surf->normals = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    Point3* position = (Point3*) par_buffer_lock(surf->coords, PAR_WRITE);
    Vector3* normal = (Vector3*) par_buffer_lock(surf->normals, PAR_WRITE);
    for (float s = 0; s < 1 - ds / 2; s += ds) {
        for (float t = 0; t < 1 - dt / 2; t += dt) {
            const float E = 0.01f;
            Point3 p = knot_fn(s, t);
            Vector3 u = P3Sub(knot_fn(s + E, t), p);
            Vector3 v = P3Sub(knot_fn(s, t + E), p);
            Vector3 n = V3Normalize(V3Cross(u, v));
            *position++ = p;
            *normal++ = n;
        }
    }
    par_buffer_unlock(surf->coords);
    par_buffer_unlock(surf->normals);

    surf->ntriangles = slices * stacks * 2;
    int indexCount = surf->ntriangles * 3;
    surf->indices = par_buffer_alloc(indexCount * 2, PAR_GPU_ELEMENTS);
    uint16_t* index = (uint16_t*) par_buffer_lock(surf->indices, PAR_WRITE);
    int v = 0;
    for (int i = 0; i < slices - 1; i++) {
        for (int j = 0; j < stacks; j++) {
            int next = (j + 1) % stacks;
            *index++ = v + next + stacks;
            *index++ = v + next;
            *index++ = v + j;
            *index++ = v + j;
            *index++ = v + j + stacks;
            *index++ = v + next + stacks;
        }
        v += stacks;
    }
    for (int j = 0; j < stacks; j++) {
        int next = (j + 1) % stacks;
        *index++ = next;
        *index++ = v + next;
        *index++ = v + j;
        *index++ = v + j;
        *index++ = j;
        *index++ = next;
    }
    par_buffer_unlock(surf->indices);
}

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = V4ScalarDiv((Vector4){78, 61, 66, 255}, 255);

    par_state_clearcolor(bgcolor);
    par_state_depthtest(1);
    par_state_cullfaces(0);
    par_shader_load_from_asset("picking.glsl");

    const float fovy = 16 * TWOPI / 180;
    const float aspect = (float) winwidth / winheight;
    const float znear = 0.1;
    const float zfar = 300;
    projection =  M4MakePerspective(fovy, aspect, znear, zfar);

    Point3 eye = {0, 0, 4};
    Point3 target = {0, 0, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    model = M4MakeIdentity();

    knot_surface.slices = 400;
    knot_surface.stacks = 100;
    float major = 8;
    float minor = 2;
    create_torus(major, minor, &knot_surface);
}

int draw()
{
    Matrix4 modelview = M4Mul(view, model);
    Matrix3 invmodelview = M4GetUpper3x3(modelview);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_draw_clear();
    par_shader_bind(P_SIMPLE);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_uniform_matrix3f(U_IMV, &invmodelview);
    par_varray_enable(knot_surface.coords, A_POSITION, 3, PAR_FLOAT, 0, 0);
    par_varray_enable(knot_surface.normals, A_NORMAL, 3, PAR_FLOAT, 0, 0);
    par_varray_bind(knot_surface.indices);
    par_draw_triangles_u16(0, knot_surface.ntriangles);
    return 1;
}

void tick(float winwidth, float winheight, float pixratio, float seconds)
{
    const float RADIANS_PER_SECOND = 1.57;
    float theta = seconds * RADIANS_PER_SECOND;
    model = M4MakeRotationY(theta);
}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_buffer_free(knot_surface.coords);
    par_buffer_free(knot_surface.indices);
    par_buffer_free(knot_surface.normals);
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
