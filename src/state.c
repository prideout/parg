#include <parg.h>
#include "pargl.h"

int _par_depthtest = 0;

void par_state_clearcolor(Vector4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}

void par_state_cullfaces(int enabled)
{
    (enabled ? glEnable : glDisable)(GL_CULL_FACE);
}

void par_state_depthtest(int enabled)
{
    (enabled ? glEnable : glDisable)(GL_DEPTH_TEST);
    _par_depthtest = enabled;
}

void par_state_blending(int enabled)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    (enabled ? glEnable : glDisable)(GL_BLEND);
    _par_depthtest = enabled;
}
