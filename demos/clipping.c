#include <par.h>
#include <parwin.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_NORMAL, "a_normal")     \
    F(U_MVP, "u_mvp")           \
    F(U_IMV, "u_imv")           \
    F(U_COLOR, "u_color")

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
Surface torus_surface;

#define TWOPI 6.28318530718

Point3 torus_fn(float major, float minor, float phi, float theta, float beta)
{
    Point3 p;
    p.x = cos(theta) * beta;
    p.y = sin(theta) * beta;
    p.z = sin(phi) * minor;
    return p;
}

void create_torus(float major, float minor, Surface* surf)
{
    int slices = surf->slices;
    int stacks = surf->stacks;
    float dphi = TWOPI / stacks;
    float dtheta = TWOPI / slices;
    int vertexCount = slices * stacks * 3;
    int vertexStride = sizeof(float) * 3;
    surf->coords = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    Point3* position = (Point3*) par_buffer_lock(surf->coords, PAR_WRITE);
    for (int slice = 0; slice < slices; slice++) {
        float theta = slice * dtheta;
        for (int stack = 0; stack < stacks; stack++) {
            float phi = stack * dphi;
            float beta = major + minor * cos(phi);
            *position++ = torus_fn(major, minor, phi, theta, beta);
        }
    }
    par_buffer_unlock(surf->coords);

    surf->normals = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    Vector3* normal = (Vector3*) par_buffer_lock(surf->normals, PAR_WRITE);
    for (int slice = 0; slice < slices; slice++) {
        float theta = slice * dtheta;
        for (int stack = 0; stack < stacks; stack++) {
            float phi = stack * dphi;
            float beta = major + minor * cos(phi);
            Point3 p = torus_fn(major, minor, phi, theta, beta);
            Point3 p1 = torus_fn(major, minor, phi + dphi * 0.5, theta, beta);
            Point3 p2 = torus_fn(major, minor, phi, theta + dtheta * 0.5, beta);
            Vector3 du = P3Sub(p1, p);
            Vector3 dv = P3Sub(p2, p);
            *normal++ = V3Normalize(V3Cross(du, dv));
        }
    }
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
    par_state_cullfaces(1);
    par_shader_load_from_asset("clipping.glsl");

    const float h = 5.0f;
    const float w = h * winwidth / winheight;
    const float znear = 65;
    const float zfar = 90;
    projection = M4MakeFrustum(-w, w, -h, h, znear, zfar);

    Point3 eye = {0, -75, 25};
    Point3 target = {0, 0, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    model = M4MakeIdentity();

    torus_surface.slices = 40;
    torus_surface.stacks = 10;
    float major = 8;
    float minor = 2;
    create_torus(major, minor, &torus_surface);
}

int draw()
{
    const Vector4 fgcolor = V4ScalarDiv((Vector4){198, 226, 233, 255}, 255);
    Matrix4 modelview = M4Mul(view, model);
    Matrix3 normals = M4GetUpper3x3(modelview);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_draw_clear();
    par_shader_bind(P_SIMPLE);
    par_uniform4f(U_COLOR, &fgcolor);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_uniform_matrix3f(U_IMV, &normals);
    par_varray_enable(torus_surface.coords, A_POSITION, 3, PAR_FLOAT, 0, 0);
    par_varray_enable(torus_surface.normals, A_NORMAL, 3, PAR_FLOAT, 0, 0);
    par_varray_bind(torus_surface.indices);
    par_draw_triangles_u16(0, torus_surface.ntriangles);
    return 1;
}

void tick(float winwidth, float winheight, float pixratio, float seconds)
{
    const float RADIANS_PER_SECOND = 3.14;
    float theta = seconds * RADIANS_PER_SECOND;
    model = M4MakeRotationZ(theta);
}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_buffer_free(torus_surface.coords);
    par_buffer_free(torus_surface.indices);
    par_buffer_free(torus_surface.normals);
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
