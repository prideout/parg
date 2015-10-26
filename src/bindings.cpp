extern "C" {
    #include <par.h>
    #include <parwin.h>
    #include "pargl.h"
    void par_asset_set_baseurl(const char* url);
    void par_asset_onload(const char* name, par_buffer* buf);
}

#include <stdio.h>
#include <string>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

static int null_tick(float, float, float, float) { return 0; }
static void null_draw() {}
static void null_dispose() { }
static void null_init(float, float, float) {}
static void null_input(par_event, float, float, float) {}
static void null_message(const char*) {}
static int _argc = 0;
static char** _argv = 0;
static float _pixscale = 1.0f;
static int _winwidth = 0;
static int _winheight = 0;
static par_window_fn_init _init = null_init;
static par_window_fn_tick _tick = null_tick;
static par_window_fn_draw _draw = null_draw;
static par_window_fn_exit _dispose = null_dispose;
static par_window_fn_input _input = null_input;
static par_window_fn_message _message = null_message;
static par_buffer* g_buffer;

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

void par_window_onmessage(par_window_fn_message fn) { _message = fn; }

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

static void message(emscripten::val msgval)
{
    std::string msg = msgval.as<std::string>();
    _message(msg.c_str());
}

static void init(emscripten::val args)
{
    message(args);
    _init(_winwidth, _winheight, _pixscale);
}

static void draw()
{
    _draw();
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        puts("OpenGL Error\n");
    }
}

static int tick(float seconds)
{
    return _tick(_winwidth, _winheight, _pixscale, seconds);
}

static void input(int evt, float x, float y, float z)
{
    _input((par_event) evt, x, y, z);
}

static int alloc(std::string id, int nbytes)
{
    using namespace emscripten;
    printf("Allocating %d bytes for %s\n", nbytes, id.c_str());
    auto parg = val::module_property("parg");
    g_buffer = par_buffer_alloc(nbytes, PAR_CPU);
    return (int) par_buffer_lock(g_buffer, PAR_WRITE);
}

static void commit(std::string id)
{
    par_buffer_unlock(g_buffer);
    par_asset_onload(id.c_str(), g_buffer);
}

void par_asset_preload(par_token id)
{
    using namespace emscripten;
    auto parg = val::module_property("parg");
    std::string name(par_token_to_string(id));
    parg.call<void>("asset_preload", name);
}

EMSCRIPTEN_BINDINGS(par)
{
    using namespace emscripten;
    struct Window {};
    class_<Window>("Window")
        .class_function("init", &init)
        .class_function("draw", &draw)
        .class_function("tick", &tick)
        .class_function("input", &input)
        .class_function("message", &message);
    struct Asset {};
    class_<Asset>("Asset")
        .class_function("alloc", &alloc)
        .class_function("commit", &commit);
    par_asset_set_baseurl("parg/");
}
