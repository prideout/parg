#include <parg.h>
#include "pargl.h"

void parg_draw_clear()
{
    int planes = GL_COLOR_BUFFER_BIT;
    if (_parg_depthtest) {
        planes |= GL_DEPTH_BUFFER_BIT;
    }
    glClear(planes);
}

void parg_draw_one_quad() { glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); }

void parg_draw_triangles(int start, int ntriangles)
{
    glDrawArrays(GL_TRIANGLES, start * 3, ntriangles * 3);
}

void parg_draw_triangles_u16(int start, int ntriangles)
{
    long offset = start * 3 * sizeof(uint16_t);
    const GLvoid* ptr = (const GLvoid*) offset;
    glDrawElements(GL_TRIANGLES, ntriangles * 3, GL_UNSIGNED_SHORT, ptr);
}

void parg_draw_instanced_triangles_u16(
    int start, int ntriangles, int ninstances)
{
    long offset = start * 3 * sizeof(uint16_t);
    const GLvoid* ptr = (const GLvoid*) offset;
    pargDrawElementsInstanced(
        GL_TRIANGLES, ntriangles * 3, GL_UNSIGNED_SHORT, ptr, ninstances);
}

void parg_draw_wireframe_triangles_u16(int start, int ntriangles)
{
#ifndef EMSCRIPTEN
    glLineWidth(2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonOffset(0.0001, -0.0001);
    glEnable(GL_POLYGON_OFFSET_LINE);
    long offset = start * 3 * sizeof(uint16_t);
    const GLvoid* ptr = (const GLvoid*) offset;
    glDrawElements(GL_TRIANGLES, ntriangles * 3, GL_UNSIGNED_SHORT, ptr);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
#endif
}

void parg_draw_lines(int nsegments)
{
    glLineWidth(2);
    glDrawArrays(GL_LINES, 0, nsegments * 2);
}

void parg_draw_points(int npoints)
{
#if defined(GL_PROGRAM_POINT_SIZE)
    glEnable(GL_PROGRAM_POINT_SIZE);
#elif defined(GL_VERTEX_PROGRAM_POINT_SIZE)
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
    glDrawArrays(GL_POINTS, 0, npoints);
}
