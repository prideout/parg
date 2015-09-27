#include <par.h>
#include "pargl.h"

void par_draw_clear()
{
    int planes = GL_COLOR_BUFFER_BIT;
    if (_par_depthtest) {
        planes |= GL_DEPTH_BUFFER_BIT;
    }
    glClear(planes);
}

void par_draw_triangles(int start, int count)
{
    glDrawArrays(GL_TRIANGLES, start * 3, count * 3);
}

void par_draw_triangles_u16(int start, int count)
{
    long offset = start * 3 * sizeof(unsigned short);
    const GLvoid* ptr = (const GLvoid*) offset;
    glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_SHORT, ptr);
}
