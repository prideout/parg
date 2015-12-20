#include <parg.h>
#include <parwin.h>
#include <assert.h>
#include <sds.h>
#include <stdio.h>
#include <string.h>

#define TOKEN_TABLE(F)          \
    F(P_TEXTURED, "p_textured") \
    F(P_HIGHP, "p_highp")       \
    F(A_POSITION, "a_position") \
    F(A_TEXCOORD, "a_texcoord") \
    F(U_MVP, "u_mvp")           \
    F(U_EYEPOS, "u_eyepos")     \
    F(U_EYEPOS_LOWPART, "u_eyepos_lowpart")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_TABLE(F)                \
    F(SHADER_MARINA, "marina.glsl")   \
    F(TEXTURE_DOGGIES, "doggies.png") \
    F(TEXTURE_ORIGIN, "origin_z02.png")
ASSET_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_LIST(F)   \
    F("marina_z05.png") \
    F("marina_z10.png") \
    F("marina_z15.png") \
    F("marina_z20.png")

#define NUM_LEVELS 4
const float gray = 0.8;
const float fovy = 16 * PARG_TWOPI / 180;
const float photo_lon = -122.3245;
const float photo_lat = 37.8743;
const int levels[NUM_LEVELS] = {5, 10, 15, 20};
int mode_highp = 1;
parg_texture* marina_textures[NUM_LEVELS];
parg_texture* origin_texture;
parg_texture* doggies_texture;
parg_buffer* lines_buffer;
DVector3 photo_position = {0};
parg_mesh* tile_mesh;
parg_mesh* photo_mesh;
float tscale;

void init(float winwidth, float winheight, float pixratio)
{
    parg_state_clearcolor((Vector4){gray, gray, gray, 1});
    parg_shader_load_from_asset(SHADER_MARINA);
    origin_texture = parg_texture_from_asset(TEXTURE_ORIGIN);
    int imgwidth, imgheight;
    parg_texture_info(origin_texture, &imgwidth, &imgheight);
    assert(imgwidth == imgheight);
    tscale = 1280 / 256;
    parg_zcam_init(1, 1, fovy);
    parg_zcam_grab_update(0.5, 0.5, 20);
    tile_mesh = parg_mesh_rectangle(1, 1);
    photo_position.x = photo_lon / 360;
    float latrad = photo_lat * PARG_PI / 180;
    float mercN = log(tan((PARG_PI / 4) + (latrad / 2)));
    photo_position.y = mercN / (2 * PARG_PI);
    for (int i = 0; i < NUM_LEVELS; i++) {
        sds name = sdsnew("marina_z");
        name = sdscatprintf(name, "%02d.png", levels[i]);
        parg_token id = parg_token_from_string(name);
        marina_textures[i] = parg_texture_from_asset(id);
        sdsfree(name);
    }
    doggies_texture = parg_texture_from_asset(TEXTURE_DOGGIES);
    parg_texture_info(doggies_texture, &imgwidth, &imgheight);
    photo_mesh = parg_mesh_rectangle(1, (float) imgheight / imgwidth);
    int nverts = 4;

    // Populate a single-precision buffer of vec2's for the lines.
    int vstride = sizeof(float) * 2;
    lines_buffer = parg_buffer_alloc(nverts * vstride, PARG_GPU_ARRAY);
    float* plines = parg_buffer_lock(lines_buffer, PARG_WRITE);
    *plines++ = photo_position.x;
    *plines++ = -1;
    *plines++ = photo_position.x;
    *plines++ = 1;
    *plines++ = -1;
    *plines++ = photo_position.y;
    *plines++ = 1;
    *plines++ = photo_position.y;
    parg_buffer_unlock(lines_buffer);
}

void draw()
{
    double scale;
    DMatrix4 view, projection, model;
    Matrix4 mvp;
    parg_zcam_dmatrices(&projection, &view);
    parg_draw_clear();

    // First, draw the map "tiles" (these aren't really slippy map tiles)
    parg_shader_bind(P_TEXTURED);
    parg_varray_enable(
        parg_mesh_coord(tile_mesh), A_POSITION, 2, PARG_FLOAT, 0, 0);
    parg_varray_enable(
        parg_mesh_uv(tile_mesh), A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    scale = tscale / pow(2, 2);
    model = DM4MakeScale((DVector3){scale, scale, scale});
    mvp = M4MakeFromDM4(DM4Mul(projection, DM4Mul(view, model)));
    parg_texture_bind(origin_texture, 0);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_draw_one_quad();
    for (int i = 0; i < NUM_LEVELS; i++) {
        scale = tscale / pow(2, levels[i]);
        model = DM4Mul(DM4MakeTranslation(photo_position),
                DM4MakeScale((DVector3){scale, scale, scale}));
        mvp = M4MakeFromDM4(DM4Mul(projection, DM4Mul(view, model)));
        parg_texture_bind(marina_textures[i], 0);
        parg_uniform_matrix4f(U_MVP, &mvp);
        parg_draw_one_quad();
    }

    // Recompute the MVP, pretending that the camera is at the origin.
    Point3 eyepos, eyepos_lowpart;
    parg_zcam_highprec(&mvp, &eyepos_lowpart, &eyepos);
    if (!mode_highp) {
        eyepos_lowpart = (Point3){0};
    }

    // Draw the crosshair lines.
    parg_shader_bind(P_HIGHP);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_uniform_point(U_EYEPOS, &eyepos);
    parg_uniform_point(U_EYEPOS_LOWPART, &eyepos_lowpart);
    parg_varray_enable(lines_buffer, A_POSITION, 2, PARG_FLOAT, 0, 0);
    parg_draw_lines(2);

    // Draw the photo.
    parg_shader_bind(P_TEXTURED);
    parg_varray_enable(
        parg_mesh_coord(photo_mesh), A_POSITION, 2, PARG_FLOAT, 0, 0);
    scale = tscale / pow(2, 25);
    model = DM4Mul(DM4MakeTranslation(photo_position),
            DM4MakeScale((DVector3){scale, scale, scale}));
    mvp = M4MakeFromDM4(DM4Mul(projection, DM4Mul(view, model)));
    parg_texture_bind(doggies_texture, 0);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_draw_one_quad();
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    parg_zcam_tick(winwidth / winheight, seconds);
    return parg_zcam_has_moved();
}

void dispose()
{
    parg_buffer_free(lines_buffer);
    parg_mesh_free(tile_mesh);
    parg_mesh_free(photo_mesh);
    parg_texture_free(origin_texture);
    parg_texture_free(doggies_texture);
    for (int i = 0; i < NUM_LEVELS; i++) {
        parg_texture_free(marina_textures[i]);
    }
}

void input(parg_event evt, float x, float y, float z)
{
    switch (evt) {
    case PARG_EVENT_DOWN:
        parg_zcam_grab_begin(x, y);
        break;
    case PARG_EVENT_UP:
        parg_zcam_grab_update(x, y, z);
        parg_zcam_grab_end();
        break;
    case PARG_EVENT_MOVE:
        parg_zcam_grab_update(x, y, z);
        break;
    default:
        break;
    }
}

void message(const char* msg)
{
    if (!strcmp(msg, "high")) {
        mode_highp = 1;
    } else if (!strcmp(msg, "low")) {
        mode_highp = 0;
    }
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PARG_TOKEN_DEFINE);
    ASSET_TABLE(PARG_ASSET_TABLE);
    ASSET_LIST(PARG_ASSET_LIST);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_ontick(tick);
    parg_window_ondraw(draw);
    parg_window_onexit(dispose);
    parg_window_oninput(input);
    parg_window_onmessage(message);
    return parg_window_exec(400, 300, 1);
}
