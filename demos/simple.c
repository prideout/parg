#include <par.h>
#include <parwin.h>
#include <pargl.h>
#include <stdio.h>

#define TOKEN_TABLE(F)   \
    F(U_MVP,   "u_mvp")    \
    F(A_POSITION, "a_position")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

void init(float winwidth, float winheight, float pixratio)
{
    glClearColor(0, 0.25, 0.5, 1.0);
    printf("%s\n", par_token_to_string(U_MVP));
    printf("%s\n", par_token_to_string(A_POSITION));
}

int draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    return 1;
}

void tick(float seconds, float winwidth, float winheight, float pixratio)
{
}

void dispose()
{
}

int main(int argc, char *argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
