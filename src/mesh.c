#include <par.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

struct par_mesh_s {
    par_buffer* coords;
    par_buffer* uvs;
    par_buffer* normals;
    par_buffer* indices;
    int ntriangles;
};

par_mesh* par_mesh_create(float* pts, int npts, uint16_t* tris, int ntris)
{
    par_mesh* surf = malloc(sizeof(struct par_mesh_s));
    surf->coords =
        par_buffer_create(pts, npts * sizeof(float) * 3, PAR_GPU_ARRAY);
    surf->uvs = 0;
    surf->normals = 0;
    surf->indices =
        par_buffer_create(tris, ntris * sizeof(uint16_t) * 3, PAR_GPU_ELEMENTS);
    surf->ntriangles = ntris;
    return surf;
}

static Point3 torus_fn(float major, float minor, float phi, float theta)
{
    float beta = major + minor * cos(phi);
    Point3 p;
    p.x = cos(theta) * beta;
    p.y = sin(theta) * beta;
    p.z = sin(phi) * minor;
    return p;
}

Point3 knot_fn(float s, float t)
{
    const float a = 0.5f;
    const float b = 0.3f;
    const float c = 0.5f;
    const float d = 0.1f;
    const float u = (1 - s) * 2 * PAR_TWOPI;
    const float v = t * PAR_TWOPI;
    const float r = a + b * cos(1.5f * u);
    const float x = r * cos(u);
    const float y = r * sin(u);
    const float z = c * sin(1.5f * u);

    Vector3 dv;
    dv.x =
        -1.5f * b * sin(1.5f * u) * cos(u) - (a + b * cos(1.5f * u)) * sin(u);
    dv.y =
        -1.5f * b * sin(1.5f * u) * sin(u) + (a + b * cos(1.5f * u)) * cos(u);
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

par_mesh* par_mesh_knot(int slices, int stacks, float major, float minor)
{
    par_mesh* surf = malloc(sizeof(struct par_mesh_s));
    float ds = 1.0f / slices;
    float dt = 1.0f / stacks;
    int vertexCount = slices * stacks * 3;
    int vertexStride = sizeof(float) * 3;
    surf->coords = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    surf->normals = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    surf->uvs = 0;
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
    return surf;
}

par_mesh* par_mesh_torus(int slices, int stacks, float major, float minor)
{
    par_mesh* surf = malloc(sizeof(struct par_mesh_s));
    float dphi = PAR_TWOPI / stacks;
    float dtheta = PAR_TWOPI / slices;
    int vertexCount = slices * stacks * 3;
    int vertexStride = sizeof(float) * 3;
    surf->coords = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    surf->uvs = 0;
    Point3* position = (Point3*) par_buffer_lock(surf->coords, PAR_WRITE);
    for (int slice = 0; slice < slices; slice++) {
        float theta = slice * dtheta;
        for (int stack = 0; stack < stacks; stack++) {
            float phi = stack * dphi;
            *position++ = torus_fn(major, minor, phi, theta);
        }
    }
    par_buffer_unlock(surf->coords);

    surf->normals = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    Vector3* normal = (Vector3*) par_buffer_lock(surf->normals, PAR_WRITE);
    for (int slice = 0; slice < slices; slice++) {
        float theta = slice * dtheta;
        for (int stack = 0; stack < stacks; stack++) {
            float phi = stack * dphi;
            Point3 p = torus_fn(major, minor, phi, theta);
            Point3 p1 = torus_fn(major, minor, phi, theta + 0.01);
            Point3 p2 = torus_fn(major, minor, phi + 0.01, theta);
            Vector3 du = P3Sub(p2, p);
            Vector3 dv = P3Sub(p1, p);
            *normal = V3Normalize(V3Cross(du, dv));
            ++normal;
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
    return surf;
}

par_mesh* par_mesh_rectangle(float width, float height)
{
    par_mesh* surf = malloc(sizeof(struct par_mesh_s));
    surf->normals = 0;
    surf->indices = 0;
    surf->ntriangles = 2;
    int vertexCount = 4;
    int vertexStride = sizeof(float) * 2;
    surf->coords = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    float* position = (float*) par_buffer_lock(surf->coords, PAR_WRITE);
    float w = width * 0.5, h = height * 0.5;
    *position++ = -w;
    *position++ = -h;
    *position++ = +w;
    *position++ = -h;
    *position++ = -w;
    *position++ = +h;
    *position++ = +w;
    *position = +h;
    par_buffer_unlock(surf->coords);
    surf->uvs = par_buffer_alloc(vertexCount * vertexStride, PAR_GPU_ARRAY);
    float* texcoord = (float*) par_buffer_lock(surf->uvs, PAR_WRITE);
    *texcoord++ = 0;
    *texcoord++ = 0;
    *texcoord++ = 1;
    *texcoord++ = 0;
    *texcoord++ = 0;
    *texcoord++ = 1;
    *texcoord++ = 1;
    *texcoord = 1;
    par_buffer_unlock(surf->uvs);
    return surf;
}

par_mesh* par_mesh_sierpinski(float width, int depth)
{
    par_mesh* surf = malloc(sizeof(struct par_mesh_s));
    surf->normals = 0;
    surf->indices = 0;
    surf->uvs = 0;
    surf->ntriangles = pow(3, depth);
    int vstride = sizeof(float) * 2;
    int ntriangles = 1;
    int nverts = ntriangles * 3;
    float height = width * sqrt(0.75);

    par_buffer* src = par_buffer_alloc(nverts * vstride, PAR_CPU);
    float* psrc = (float*) par_buffer_lock(src, PAR_WRITE);
    *psrc++ = 0;
    *psrc++ = height * 0.5;
    *psrc++ = width * 0.5;
    *psrc++ = -height * 0.5;
    *psrc++ = -width * 0.5;
    *psrc++ = -height * 0.5;
    par_buffer_unlock(src);

#define WRITE_TRIANGLE(a, b, c) \
    *pdst++ = x[a];             \
    *pdst++ = y[a];             \
    *pdst++ = x[b];             \
    *pdst++ = y[b];             \
    *pdst++ = x[c];             \
    *pdst++ = y[c]

    float x[6];
    float y[6];
    while (depth--) {
        ntriangles *= 3;
        nverts = ntriangles * 3;
        par_buffer* dst = par_buffer_alloc(nverts * vstride, PAR_CPU);
        float* pdst = par_buffer_lock(dst, PAR_WRITE);
        const float* psrc = par_buffer_lock(src, PAR_READ);
        for (int i = 0; i < ntriangles / 3; i++) {
            x[0] = *psrc++;
            y[0] = *psrc++;
            x[1] = *psrc++;
            y[1] = *psrc++;
            x[2] = *psrc++;
            y[2] = *psrc++;
            x[3] = 0.5 * (x[0] + x[1]);
            y[3] = 0.5 * (y[0] + y[1]);
            x[4] = 0.5 * (x[1] + x[2]);
            y[4] = 0.5 * (y[1] + y[2]);
            x[5] = 0.5 * (x[0] + x[2]);
            y[5] = 0.5 * (y[0] + y[2]);
            WRITE_TRIANGLE(0, 3, 5);
            WRITE_TRIANGLE(3, 1, 4);
            WRITE_TRIANGLE(5, 4, 2);
        }
        par_buffer_unlock(src);
        par_buffer_unlock(dst);
        par_buffer_free(src);
        src = dst;
    }

    assert(surf->ntriangles == ntriangles);
    surf->coords = par_buffer_alloc(nverts * vstride, PAR_GPU_ARRAY);
    float* pdst = par_buffer_lock(surf->coords, PAR_WRITE);
    psrc = par_buffer_lock(src, PAR_READ);
    memcpy(pdst, psrc, nverts * vstride);
    par_buffer_unlock(src);
    par_buffer_unlock(surf->coords);
    par_buffer_free(src);
    return surf;
}

void par_mesh_free(par_mesh* m)
{
    par_buffer_free(m->coords);
    par_buffer_free(m->indices);
    par_buffer_free(m->normals);
    par_buffer_free(m->uvs);
    free(m);
}

par_buffer* par_mesh_coord(par_mesh* m) { return m->coords; }

par_buffer* par_mesh_uv(par_mesh* m) { return m->uvs; }

par_buffer* par_mesh_norml(par_mesh* m) { return m->normals; }

par_buffer* par_mesh_index(par_mesh* m) { return m->indices; }

int par_mesh_ntriangles(par_mesh* m) { return m->ntriangles; }
