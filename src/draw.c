#include <parg.h>
#include "pargl.h"

void par_draw_clear()
{
    int planes = GL_COLOR_BUFFER_BIT;
    if (_par_depthtest) {
        planes |= GL_DEPTH_BUFFER_BIT;
    }
    glClear(planes);
}

void par_draw_one_quad() { glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); }

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

void par_draw_wireframe_triangles_u16(int start, int count)
{
    #ifndef EMSCRIPTEN
    glLineWidth(2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonOffset(0.0001, -0.0001);
    glEnable(GL_POLYGON_OFFSET_LINE);
    long offset = start * 3 * sizeof(unsigned short);
    const GLvoid* ptr = (const GLvoid*) offset;
    glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_SHORT, ptr);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    #endif
}

void par_draw_lines(int nsegments)
{
    glLineWidth(2);
    glDrawArrays(GL_LINES, 0, nsegments * 2);
}

void par_draw_points(int npoints)
{
#if defined(GL_PROGRAM_POINT_SIZE)
    glEnable(GL_PROGRAM_POINT_SIZE);
#elif defined(GL_VERTEX_PROGRAM_POINT_SIZE)
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
    glDrawArrays(GL_POINTS, 0, npoints);
}
