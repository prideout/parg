#include <par.h>
#include <parwin.h>
#include <pargl.h>
#include <stdio.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(U_COLOR, "u_color")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

void init(float winwidth, float winheight, float pixratio)
{
    glClearColor(0, 0.25, 0.5, 1.0);
    par_shader_load_from_asset("simple.glsl");
}

int draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    par_shader_bind(P_SIMPLE);  // EYEBALL implement this.
    return 1;
}

void tick(float seconds, float winwidth, float winheight, float pixratio) {}

void dispose()
{
    // par_shader_free(P_SIMPLE); // TODO provide this
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
