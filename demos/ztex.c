#include <parg.h>
#include <parwin.h>
#include <assert.h>
#include <sds.h>
#include <stdio.h>

#define PAR_MSQUARES_IMPLEMENTATION
#include <par/par_msquares.h>

#define TOKEN_TABLE(F)          \
    F(P_LANDMASS, "p_landmass") \
    F(P_OCEAN, "p_ocean")       \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(U_MAGNIFICATION, "u_magnification")
TOKEN_TABLE(PAR_TOKEN_DECLARE);

#define ASSET_TABLE(F)              \
    F(SHADER_DEFAULT, "ztex.glsl")  \
    F(TEXTURE_PAPER, "paper.png")   \
    F(TEXTURE_EUROPE, "europe.png") \
    F(TEXTURE_OCEAN, "water.png")
ASSET_TABLE(PAR_TOKEN_DECLARE);

const float fovy = 16 * PAR_TWOPI / 180;
int mode_highp = 1;
par_mesh* landmass_mesh;
par_mesh* ocean_mesh;
par_texture* ocean_texture;
par_texture* paper_texture;

void init(float winwidth, float winheight, float pixratio)
{
    par_state_clearcolor((Vector4){0.43, 0.61, 0.8, 1});
    par_state_cullfaces(1);
    par_state_depthtest(0);
    par_shader_load_from_asset(SHADER_DEFAULT);
    par_zcam_init(1, 1, fovy);
    par_zcam_grab_update(0.5, 0.5, 30);
    ocean_texture = par_texture_from_asset(TEXTURE_OCEAN);
    paper_texture = par_texture_from_asset(TEXTURE_PAPER);

    // Decode the europe image and pass it into msquares.
    int* rawdata;
    par_buffer* colorbuf =
        par_buffer_slurp_asset(TEXTURE_EUROPE, (void*) &rawdata);
    int width = *rawdata++;
    int height = *rawdata++;
    int ncomps = *rawdata++;
    par_texture_fliprows(rawdata, width * ncomps, height);
    par_msquares_meshlist* mlist = par_msquares_color((par_byte*) rawdata,
            width, height, 16, 0xffa0d9e1, 4,
            PAR_MSQUARES_DUAL | PAR_MSQUARES_HEIGHTS | PAR_MSQUARES_SIMPLIFY);
    printf("%d meshes\n", par_msquares_get_count(mlist));

    par_msquares_mesh const* mesh;
    mesh = par_msquares_get_mesh(mlist, 0);
    landmass_mesh = par_mesh_create(
        mesh->points, mesh->npoints, mesh->triangles, mesh->ntriangles);

    mesh = par_msquares_get_mesh(mlist, 1);
    ocean_mesh = par_mesh_create(
        mesh->points, mesh->npoints, mesh->triangles, mesh->ntriangles);

    par_buffer_unlock(colorbuf);
    par_msquares_free(mlist);
}

void draw()
{
    DMatrix4 view, projection;
    par_zcam_dmatrices(&projection, &view);
    DMatrix4 model = DM4MakeTranslation((DVector3){-0.5, -0.5, -1});
    Matrix4 mvp = M4MakeFromDM4(DM4Mul(projection, DM4Mul(view, model)));
    float mag = par_zcam_get_magnification();

    par_draw_clear();
    par_shader_bind(P_LANDMASS);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_uniform1f(U_MAGNIFICATION, mag);
    par_texture_bind(paper_texture, 0);
    par_varray_bind(par_mesh_index(landmass_mesh));
    par_varray_enable(
        par_mesh_coord(landmass_mesh), A_POSITION, 3, PAR_FLOAT, 0, 0);
    par_draw_triangles_u16(0, par_mesh_ntriangles(ocean_mesh));
    par_shader_bind(P_OCEAN);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_uniform1f(U_MAGNIFICATION, mag);
    par_texture_bind(ocean_texture, 0);
    par_varray_bind(par_mesh_index(ocean_mesh));
    par_varray_enable(
        par_mesh_coord(ocean_mesh), A_POSITION, 3, PAR_FLOAT, 0, 0);
    par_draw_triangles_u16(0, par_mesh_ntriangles(ocean_mesh));
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    par_zcam_tick(winwidth / winheight, seconds);
    return par_zcam_has_moved();
}

void dispose()
{
    par_mesh_free(landmass_mesh);
    par_mesh_free(ocean_mesh);
    par_texture_free(ocean_texture);
    par_texture_free(paper_texture);
}

void input(par_event evt, float x, float y, float z)
{
    switch (evt) {
    case PAR_EVENT_KEYPRESS:
        if ((char) x == ' ') {
            mode_highp = !mode_highp;
            printf("Precision %s.\n", mode_highp ? "on" : "off");
        }
        break;
    case PAR_EVENT_DOWN:
        par_zcam_grab_begin(x, y);
        break;
    case PAR_EVENT_UP:
        par_zcam_grab_update(x, y, z);
        par_zcam_grab_end();
        break;
    case PAR_EVENT_MOVE:
        par_zcam_grab_update(x, y, z);
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
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    ASSET_TABLE(PAR_ASSET_TABLE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    par_window_oninput(input);
    par_window_onmessage(message);
    return par_window_exec(400, 300, 1);
}
