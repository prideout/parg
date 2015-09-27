#include <par.h>
#include <pargl.h>

void par_state_clearcolor(Vector4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}

void par_state_cullfaces(int enabled)
{
    (enabled ? glEnable : glDisable)(GL_CULL_FACE);
}
