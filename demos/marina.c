#include <par.h>
#include <parwin.h>
#include <assert.h>
#include <sds.h>

#define TOKEN_TABLE(F)          \
    F(P_TEXTURED, "p_textured") \
    F(P_SOLID, "p_solid")       \
    F(A_POSITION, "a_position") \
    F(A_TEXCOORD, "a_texcoord") \
    F(U_MVP, "u_mvp")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

#define NUM_LEVELS 4

const float gray = 0.8;
const float fovy = 16 * PAR_TWOPI / 180;
const float lon = -122.3245;
const float lat = 37.8743;
const int levels[NUM_LEVELS] = {5, 10, 15, 20};

par_texture* marina_textures[NUM_LEVELS];
par_texture* origin_texture;
par_texture* doggies_texture;
par_buffer* lines_buffer;
Vector3 translation = {0};
par_mesh* tile_mesh;
par_mesh* photo_mesh;
float tscale;

void init(float winwidth, float winheight, float pixratio)
{
    par_state_clearcolor((Vector4){gray, gray, gray, 1});
    par_shader_load_from_asset("marina.glsl");
    origin_texture = par_texture_from_asset("origin_z02.png");
    int imgwidth, imgheight;
    par_texture_info(origin_texture, &imgwidth, &imgheight);
    assert(imgwidth == imgheight);
    tscale = imgwidth / 256;
    par_zcam_init(1, 1, fovy);
    par_zcam_grab_update(0.5, 0.5, 20);
    tile_mesh = par_mesh_create_rectangle(1, 1);
    translation.x = lon / 360;
    float latrad = lat * PAR_PI / 180;
    float mercN = log(tan((PAR_PI / 4) + (latrad / 2)));
    translation.y = mercN / (2 * PAR_PI);
    for (int i = 0; i < NUM_LEVELS; i++) {
        sds name = sdsnew("marina_z");
        name = sdscatprintf(name, "%02d.png", levels[i]);
        marina_textures[i] = par_texture_from_asset(name);
        sdsfree(name);
    }
    doggies_texture = par_texture_from_asset("doggies.png");
    par_texture_info(doggies_texture, &imgwidth, &imgheight);
    photo_mesh = par_mesh_create_rectangle(1, (float) imgheight / imgwidth);
    int nverts = 4;
    int vstride = sizeof(float) * 2;
    lines_buffer = par_buffer_alloc(nverts * vstride, PAR_GPU_ARRAY);
    float* plines = par_buffer_lock(lines_buffer, PAR_WRITE);
    *plines++ = translation.x;
    *plines++ = -1;
    *plines++ = translation.x;
    *plines++ = 1;
    *plines++ = -1;
    *plines++ = translation.y;
    *plines++ = 1;
    *plines++ = translation.y;
    par_buffer_unlock(lines_buffer);
}

int draw()
{
    float scale;
    Matrix4 view, projection, model, mvp;
    par_zcam_matrices(&projection, &view);
    par_draw_clear();
    par_shader_bind(P_TEXTURED);
    par_varray_enable(
        par_mesh_coord(tile_mesh), A_POSITION, 2, PAR_FLOAT, 0, 0);
    par_varray_enable(par_mesh_uv(tile_mesh), A_TEXCOORD, 2, PAR_FLOAT, 0, 0);
    scale = tscale / pow(2, 2);
    model = M4MakeScale((Vector3){scale, scale, scale});
    mvp = M4Mul(projection, M4Mul(view, model));
    par_texture_bind(origin_texture, 0);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_draw_one_quad();
    for (int i = 0; i < NUM_LEVELS; i++) {
        scale = tscale / pow(2, levels[i]);
        model = M4Mul(M4MakeTranslation(translation),
                M4MakeScale((Vector3){scale, scale, scale}));
        mvp = M4Mul(projection, M4Mul(view, model));
        par_texture_bind(marina_textures[i], 0);
        par_uniform_matrix4f(U_MVP, &mvp);
        par_draw_one_quad();
    }

    par_shader_bind(P_SOLID);
    mvp = M4Mul(projection, view);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_varray_enable(lines_buffer, A_POSITION, 2, PAR_FLOAT, 0, 0);
    par_draw_lines(2);

    par_shader_bind(P_TEXTURED);
    par_varray_enable(
        par_mesh_coord(photo_mesh), A_POSITION, 2, PAR_FLOAT, 0, 0);
    scale = tscale / pow(2, 25);
    model = M4Mul(M4MakeTranslation(translation),
            M4MakeScale((Vector3){scale, scale, scale}));
    mvp = M4Mul(projection, M4Mul(view, model));
    par_texture_bind(doggies_texture, 0);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_draw_one_quad();

    return 1;
}

void tick(float winwidth, float winheight, float pixratio, float seconds)
{
    par_zcam_tick(winwidth / winheight, seconds);
}

void dispose()
{
    par_buffer_free(lines_buffer);
    par_mesh_free(tile_mesh);
    par_mesh_free(photo_mesh);
    par_texture_free(origin_texture);
    par_texture_free(doggies_texture);
    for (int i = 0; i < NUM_LEVELS; i++) {
        par_texture_free(marina_textures[i]);
    }
}

void input(par_event evt, float x, float y, float z)
{
    switch (evt) {
    case PAR_EVENT_DOWN:
        par_zcam_grab_begin(x, y);
        break;
    case PAR_EVENT_UP:
        par_zcam_grab_update(x, y, z);
        par_zcam_grab_release();
        break;
    case PAR_EVENT_MOVE:
        par_zcam_grab_update(x, y, z);
        break;
    }
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    par_window_oninput(input);
    return par_window_exec(600, 400, 1);
}
