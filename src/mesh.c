#define PAR_SHAPES_IMPLEMENTATION
#include <par/par_shapes.h>
#include <parg.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "internal.h"

parg_mesh* parg_mesh_create(float* pts, int npts, uint16_t* tris, int ntris)
{
    parg_mesh* surf = malloc(sizeof(struct parg_mesh_s));
    surf->coords =
        parg_buffer_create(pts, npts * sizeof(float) * 3, PARG_GPU_ARRAY);
    surf->uvs = 0;
    surf->normals = 0;
    surf->indices = parg_buffer_create(
        tris, ntris * sizeof(uint16_t) * 3, PARG_GPU_ELEMENTS);
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
    const float u = (1 - s) * 2 * PARG_TWOPI;
    const float v = t * PARG_TWOPI;
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

parg_mesh* parg_mesh_knot(int slices, int stacks, float major, float minor)
{
    parg_mesh* surf = malloc(sizeof(struct parg_mesh_s));
    float ds = 1.0f / slices;
    float dt = 1.0f / stacks;
    int vertexCount = slices * stacks * 3;
    int vertexStride = sizeof(float) * 3;
    surf->coords =
        parg_buffer_alloc(vertexCount * vertexStride, PARG_GPU_ARRAY);
    surf->normals =
        parg_buffer_alloc(vertexCount * vertexStride, PARG_GPU_ARRAY);
    surf->uvs = 0;
    Point3* position = (Point3*) parg_buffer_lock(surf->coords, PARG_WRITE);
    Vector3* normal = (Vector3*) parg_buffer_lock(surf->normals, PARG_WRITE);
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
    parg_buffer_unlock(surf->coords);
    parg_buffer_unlock(surf->normals);

    surf->ntriangles = slices * stacks * 2;
    int indexCount = surf->ntriangles * 3;
    surf->indices = parg_buffer_alloc(indexCount * 2, PARG_GPU_ELEMENTS);
    uint16_t* index = (uint16_t*) parg_buffer_lock(surf->indices, PARG_WRITE);
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
    parg_buffer_unlock(surf->indices);
    return surf;
}

parg_mesh* parg_mesh_torus(int slices, int stacks, float major, float minor)
{
    parg_mesh* surf = malloc(sizeof(struct parg_mesh_s));
    float dphi = PARG_TWOPI / stacks;
    float dtheta = PARG_TWOPI / slices;
    int vertexCount = slices * stacks * 3;
    int vertexStride = sizeof(float) * 3;
    surf->coords =
        parg_buffer_alloc(vertexCount * vertexStride, PARG_GPU_ARRAY);
    surf->uvs = 0;
    Point3* position = (Point3*) parg_buffer_lock(surf->coords, PARG_WRITE);
    for (int slice = 0; slice < slices; slice++) {
        float theta = slice * dtheta;
        for (int stack = 0; stack < stacks; stack++) {
            float phi = stack * dphi;
            *position++ = torus_fn(major, minor, phi, theta);
        }
    }
    parg_buffer_unlock(surf->coords);

    surf->normals =
        parg_buffer_alloc(vertexCount * vertexStride, PARG_GPU_ARRAY);
    Vector3* normal = (Vector3*) parg_buffer_lock(surf->normals, PARG_WRITE);
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
    parg_buffer_unlock(surf->normals);

    surf->ntriangles = slices * stacks * 2;
    int indexCount = surf->ntriangles * 3;
    surf->indices = parg_buffer_alloc(indexCount * 2, PARG_GPU_ELEMENTS);
    uint16_t* index = (uint16_t*) parg_buffer_lock(surf->indices, PARG_WRITE);
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
    parg_buffer_unlock(surf->indices);
    return surf;
}

parg_mesh* parg_mesh_aar(parg_aar rect)
{
    parg_mesh* surf = malloc(sizeof(struct parg_mesh_s));
    surf->normals = 0;
    surf->indices = 0;
    surf->ntriangles = 2;
    int vertexCount = 4;
    int vertexStride = sizeof(float) * 2;
    surf->coords =
        parg_buffer_alloc(vertexCount * vertexStride, PARG_GPU_ARRAY);
    float* position = (float*) parg_buffer_lock(surf->coords, PARG_WRITE);
    *position++ = rect.left;
    *position++ = rect.bottom;
    *position++ = rect.right;
    *position++ = rect.bottom;
    *position++ = rect.left;
    *position++ = rect.top;
    *position++ = rect.right;
    *position = rect.top;
    parg_buffer_unlock(surf->coords);
    surf->uvs = parg_buffer_alloc(vertexCount * vertexStride, PARG_GPU_ARRAY);
    float* texcoord = (float*) parg_buffer_lock(surf->uvs, PARG_WRITE);
    *texcoord++ = 0;
    *texcoord++ = 0;
    *texcoord++ = 1;
    *texcoord++ = 0;
    *texcoord++ = 0;
    *texcoord++ = 1;
    *texcoord++ = 1;
    *texcoord = 1;
    parg_buffer_unlock(surf->uvs);
    return surf;
}

parg_mesh* parg_mesh_rectangle(float width, float height)
{
    float w = width * 0.5, h = height * 0.5;
    return parg_mesh_aar((parg_aar){-w, -h, w, h});
}

parg_mesh* parg_mesh_sierpinski(float width, int depth)
{
    parg_mesh* surf = malloc(sizeof(struct parg_mesh_s));
    surf->normals = 0;
    surf->indices = 0;
    surf->uvs = 0;
    surf->ntriangles = pow(3, depth);
    int vstride = sizeof(float) * 2;
    int ntriangles = 1;
    int nverts = ntriangles * 3;
    float height = width * sqrt(0.75);

    parg_buffer* src = parg_buffer_alloc(nverts * vstride, PARG_CPU);
    float* psrc = (float*) parg_buffer_lock(src, PARG_WRITE);
    *psrc++ = 0;
    *psrc++ = height * 0.5;
    *psrc++ = width * 0.5;
    *psrc++ = -height * 0.5;
    *psrc++ = -width * 0.5;
    *psrc++ = -height * 0.5;
    parg_buffer_unlock(src);

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
        parg_buffer* dst = parg_buffer_alloc(nverts * vstride, PARG_CPU);
        float* pdst = parg_buffer_lock(dst, PARG_WRITE);
        const float* psrc = parg_buffer_lock(src, PARG_READ);
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
        parg_buffer_unlock(src);
        parg_buffer_unlock(dst);
        parg_buffer_free(src);
        src = dst;
    }

    assert(surf->ntriangles == ntriangles);
    surf->coords = parg_buffer_alloc(nverts * vstride, PARG_GPU_ARRAY);
    float* pdst = parg_buffer_lock(surf->coords, PARG_WRITE);
    psrc = parg_buffer_lock(src, PARG_READ);
    memcpy(pdst, psrc, nverts * vstride);
    parg_buffer_unlock(src);
    parg_buffer_unlock(surf->coords);
    parg_buffer_free(src);
    return surf;
}

void parg_mesh_free(parg_mesh* m)
{
    if (!m) {
        return;
    }
    parg_buffer_free(m->coords);
    parg_buffer_free(m->indices);
    parg_buffer_free(m->normals);
    parg_buffer_free(m->uvs);
    free(m);
}

parg_buffer* parg_mesh_coord(parg_mesh* m) { return m->coords; }

parg_buffer* parg_mesh_uv(parg_mesh* m) { return m->uvs; }

parg_buffer* parg_mesh_norml(parg_mesh* m) { return m->normals; }

parg_buffer* parg_mesh_index(parg_mesh* m) { return m->indices; }

int parg_mesh_ntriangles(parg_mesh* m) { return m->ntriangles; }

parg_mesh* parg_mesh_from_asset(parg_token id)
{
    parg_mesh* surf = calloc(sizeof(struct parg_mesh_s), 1);
    int* rawdata;
    parg_buffer* objbuf = parg_buffer_slurp_asset(id, (void*) &rawdata);
    parg_load_obj(surf, objbuf);
    parg_buffer_free(objbuf);
    return surf;
}

parg_mesh* parg_mesh_from_file(const char* filepath)
{
    parg_mesh* surf = calloc(sizeof(struct parg_mesh_s), 1);
    parg_buffer* objbuf = parg_buffer_from_file(filepath);
    parg_load_obj(surf, objbuf);
    parg_buffer_free(objbuf);
    return surf;
}

parg_mesh* parg_mesh_from_shape(struct par_shapes_mesh_s const* src)
{
    parg_mesh* dst = calloc(sizeof(struct parg_mesh_s), 1);
    dst->coords = parg_buffer_alloc(4 * 3 * src->npoints, PARG_GPU_ARRAY);
    float* pcoords = (float*) parg_buffer_lock(dst->coords, PARG_WRITE);
    memcpy(pcoords, src->points, 4 * 3 * src->npoints);
    parg_buffer_unlock(dst->coords);
    if (src->tcoords) {
        dst->uvs = parg_buffer_alloc(4 * 2 * src->npoints, PARG_GPU_ARRAY);
        float* puvs = (float*) parg_buffer_lock(dst->uvs, PARG_WRITE);
        memcpy(puvs, src->tcoords, 4 * 2 * src->npoints);
        parg_buffer_unlock(dst->uvs);
    }
    if (src->normals) {
        dst->normals = parg_buffer_alloc(4 * 3 * src->npoints, PARG_GPU_ARRAY);
        float* pnorms = (float*) parg_buffer_lock(dst->normals, PARG_WRITE);
        memcpy(pnorms, src->normals, 4 * 3 * src->npoints);
        parg_buffer_unlock(dst->normals);
    }
    dst->indices =
        parg_buffer_alloc(2 * 3 * src->ntriangles, PARG_GPU_ELEMENTS);
    uint16_t* ptris = (uint16_t*) parg_buffer_lock(dst->indices, PARG_WRITE);
    memcpy(ptris, src->triangles, 2 * 3 * src->ntriangles);
    parg_buffer_unlock(dst->indices);
    dst->ntriangles = src->ntriangles;
    return dst;
}

void parg_mesh_compute_normals(parg_mesh* mesh)
{
    par_shapes_mesh m = {0};
    int nbytes = parg_buffer_length(mesh->coords);
    m.points = (float*) parg_buffer_lock(mesh->coords, PARG_READ);
    m.triangles = (uint16_t*) parg_buffer_lock(mesh->indices, PARG_READ);
    m.npoints = nbytes / 12;
    m.ntriangles = mesh->ntriangles;
    par_shapes_compute_normals(&m);
    mesh->normals = parg_buffer_create(m.normals, nbytes, PARG_CPU);
    parg_buffer_unlock(mesh->coords);
    parg_buffer_unlock(mesh->indices);
    free(m.normals);
}

parg_buffer* parg_buffer_to_gpu(parg_buffer* cpubuf, parg_buffer_type memtype)
{
    int nbytes = parg_buffer_length(cpubuf);
    void* bytes = parg_buffer_lock(cpubuf, PARG_READ);
    parg_buffer* gpubuf = parg_buffer_create(bytes, nbytes, memtype);
    parg_buffer_unlock(cpubuf);
    return gpubuf;
}

void parg_mesh_send_to_gpu(parg_mesh* mesh)
{
    parg_buffer* coords = parg_buffer_to_gpu(mesh->coords, PARG_GPU_ARRAY);
    parg_buffer_free(mesh->coords);
    mesh->coords = coords;
    parg_buffer* indices = parg_buffer_to_gpu(mesh->indices, PARG_GPU_ELEMENTS);
    parg_buffer_free(mesh->indices);
    mesh->indices = indices;
    if (mesh->uvs) {
        parg_buffer* uvs = parg_buffer_to_gpu(mesh->uvs, PARG_GPU_ARRAY);
        parg_buffer_free(mesh->uvs);
        mesh->uvs = uvs;
    }
    if (mesh->normals) {
        parg_buffer* normals =
            parg_buffer_to_gpu(mesh->normals, PARG_GPU_ARRAY);
        parg_buffer_free(mesh->normals);
        mesh->normals = normals;
    }
}
