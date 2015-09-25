#include <parwin.h>
#include <pargl.h>

void init(float winwidth, float winheight, float pixratio)
{
    glClearColor(0, 0.25, 0.5, 1.0);
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
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
