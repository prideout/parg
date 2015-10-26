#include <par.h>
#include <parwin.h>

/*

   https://www.shadertoy.com/view/llBGz1

   //    Sphere s = Sphere(1.0, vec3(0, 1, 0)); // radius, center
   //    Plane p  = Plane(0.0, vec3(0, 1, 0)); // offset, normal
   //    Camera c = vec3(0, 1.1, 4),  target: vec3(0, 1.1, 0), up: (0,1,0)

   vec3 sunCol = vec3(1.);
   vec3 sunDir = normalize(vec3(-2., 1., 1.));
   vec3 skyColor(vec3 d)
   {
    return vec3(0.0,0.125,0.25);
   }

   float pow5(float x) { return x * x * x * x * x; }

   // Schlick approximation
   float fresnel(vec3 h, vec3 v, float f0)
   {
   return pow5(1. - clamp(dot(h, v), 0., 1.)) * (1. - f0) + f0;
   }

 */

static const int VSYNC = 1;
static const float ASPECT = 1.85;
static const int HEIGHT = 500;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = V4ScalarDiv((Vector4){78, 61, 66, 255}, 255);
    par_state_clearcolor(bgcolor);
}

void draw()
{
    par_draw_clear();
}

int tick(float seconds, float winwidth, float winheight, float pixratio)
{
    return 1;
}

void dispose() {}

int main(int argc, char* argv[])
{
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    return par_window_exec(ASPECT * HEIGHT, HEIGHT, VSYNC);
}
