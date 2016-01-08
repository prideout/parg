#include "parg.h"
#include "internal.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;

class StubReader : public tinyobj::MaterialReader {
public:
    StubReader() {}
    bool operator()(
        const string &matId,
        vector<tinyobj::material_t> &materials,
        map<string, int> &matMap,
        string &err) {
        return true;
    }
};

void parg_load_obj(parg_mesh* dst, parg_buffer* buffer)
{
    StubReader stubreader;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string err;
    const char* bview = (const char*) parg_buffer_lock(buffer, PARG_READ);
    stringstream ss(bview);
    tinyobj::LoadObj(shapes, materials, err, ss, stubreader);
    parg_assert(shapes.size() == 1, "OBJ file must have 1 shape.");
    const tinyobj::mesh_t& src = shapes[0].mesh;

    // Positions
    dst->coords = parg_buffer_alloc(4 * src.positions.size(), PARG_CPU);
    float* pcoords = (float*) parg_buffer_lock(dst->coords, PARG_WRITE);
    memcpy(pcoords, src.positions.data(), 4 * src.positions.size());
    parg_buffer_unlock(dst->coords);

    // Normals
    if (src.normals.size() > 0) {
        dst->normals = parg_buffer_alloc(4 * src.normals.size(), PARG_CPU);
        float* pnorms = (float*) parg_buffer_lock(dst->normals, PARG_WRITE);
        memcpy(pnorms, src.normals.data(), 4 * src.normals.size());
        parg_buffer_unlock(dst->normals);
    }

    // Texture coordinates
    if (src.texcoords.size() > 0) {
        dst->uvs = parg_buffer_alloc(4 * src.texcoords.size(), PARG_CPU);
        float* puvs = (float*) parg_buffer_lock(dst->uvs, PARG_WRITE);
        memcpy(puvs, src.texcoords.data(), 4 * src.texcoords.size());
        parg_buffer_unlock(dst->uvs);
    }

    // Triangles
    dst->indices = parg_buffer_alloc(2 * src.indices.size(), PARG_CPU);
    uint16_t* ptris = (uint16_t*) parg_buffer_lock(dst->indices, PARG_WRITE);
    for (size_t i = 0; i < src.indices.size(); i++) {
        *ptris++ = src.indices[i];
    }
    parg_buffer_unlock(dst->indices);

    // Finalize
    dst->ntriangles = (int) src.indices.size() / 3;
    parg_buffer_unlock(buffer);
}
