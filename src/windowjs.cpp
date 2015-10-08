extern "C" {
    #include <par.h>
    #include <parwin.h>
    #include "pargl.h"
}

#include <stdio.h>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

static int _argc = 0;
static char** _argv = 0;
static float _pixscale = 1.0f;
static int _winwidth = 0;
static int _winheight = 0;
static par_window_fn_init _init = 0;
static par_window_fn_tick _tick = 0;
static par_window_fn_draw _draw = 0;
static par_window_fn_exit _dispose = 0;
static par_window_fn_input _input = 0;

void par_window_setargs(int argc, char* argv[])
{
    _argc = argc;
    _argv = argv;
}

void par_window_oninit(par_window_fn_init fn) { _init = fn; }

void par_window_ontick(par_window_fn_tick fn) { _tick = fn; }

void par_window_ondraw(par_window_fn_draw fn) { _draw = fn; }

void par_window_onexit(par_window_fn_exit fn) { _dispose = fn; }

void par_window_oninput(par_window_fn_input fn) { _input = fn; }

int par_window_exec(float winwidth, float winheight, int vsync)
{
    _winwidth = winwidth;
    _winheight = winheight;
    EM_ASM_ARGS({
        Module.par_window_dims = [];
        Module.par_window_dims[0] = $0;
        Module.par_window_dims[1] = $1;
    }, winwidth, winheight);
    return 0;
}

static void init()
{
    if (_init) {
        _init(_winwidth, _winheight, _pixscale);
    }
}

static void draw()
{
    if (_draw && _draw()) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            puts("OpenGL Error\n");
        }
    }
}

static void tick(float seconds)
{
    if (_tick) {
        _tick(_winwidth, _winheight, _pixscale, seconds);
    }
}

static void input(int evt, float x, float y, float z)
{
    if (_input) {
        _input((par_event) evt, x, y, z);
    }
}

EMSCRIPTEN_BINDINGS(par)
{
    using namespace emscripten;
    struct Window {};
    class_<Window>("Window")
        .class_function("init", &init)
        .class_function("draw", &draw)
        .class_function("tick", &tick)
        .class_function("input", &input);
}
