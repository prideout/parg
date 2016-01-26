extern "C" {
    #include <parg.h>
    #include <parwin.h>
    #include "pargl.h"
    void parg_asset_set_baseurl(const char* url);
    void parg_asset_onload(const char* name, parg_buffer* buf);
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
static void null_input(parg_event, float, float, float) {}
static void null_message(const char*) {}
static int _argc = 0;
static char** _argv = 0;
static float _pixscale = 1.0f;
static int _winwidth = 0;
static int _winheight = 0;
static parg_window_fn_init _init = null_init;
static parg_window_fn_tick _tick = null_tick;
static parg_window_fn_draw _draw = null_draw;
static parg_window_fn_exit _dispose = null_dispose;
static parg_window_fn_input _input = null_input;
static parg_window_fn_message _message = null_message;
static parg_buffer* g_buffer;

static emscripten::val* get_glext()
{
    static emscripten::val* glext = 0;
    if (glext) {
        return glext;
    }
    auto parg = emscripten::val::module_property("parg");
    emscripten::val GLctx = parg["GLctx"];
    assert(GLctx.as<bool>() && "Can't find GLctx");
    emscripten::val v = GLctx.call<emscripten::val>("getExtension",
        std::string("ANGLE_instanced_arrays"));
    assert(v.as<bool>() && "Instancing extension not available.");
    return glext = new emscripten::val(v);
}

void pargVertexAttribDivisor(GLuint a, GLuint b)
{
    get_glext()->call<void>("vertexAttribDivisorANGLE", a, b);
}

void pargDrawElementsInstanced(GLenum a, GLsizei b, GLenum c, const void * d,
    GLsizei e)
{
    get_glext()->call<void>(
        "drawElementsInstancedANGLE", a, b, c, (int) d, (int) e);
}

void parg_window_setargs(int argc, char* argv[])
{
    _argc = argc;
    _argv = argv;
}

void parg_window_oninit(parg_window_fn_init fn) { _init = fn; }

void parg_window_ontick(parg_window_fn_tick fn) { _tick = fn; }

void parg_window_ondraw(parg_window_fn_draw fn) { _draw = fn; }

void parg_window_onexit(parg_window_fn_exit fn) { _dispose = fn; }

void parg_window_oninput(parg_window_fn_input fn) { _input = fn; }

void parg_window_onmessage(parg_window_fn_message fn) { _message = fn; }

int parg_window_exec(float winwidth, float winheight, int vsync, int aa)
{
    _winwidth = winwidth;
    _winheight = winheight;
    EM_ASM_ARGS({
        Module.parg_window_dims = [];
        Module.parg_window_dims[0] = $0;
        Module.parg_window_dims[1] = $1;
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
    #if 0
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        puts("OpenGL Error\n");
    }
    #endif
}

static int tick(float seconds, float pixscale)
{
    _pixscale = pixscale;
    return _tick(_winwidth, _winheight, _pixscale, seconds);
}

static void input(int evt, float x, float y, float z)
{
    _input((parg_event) evt, x, y, z);
}

static int alloc(std::string id, int nbytes)
{
    using namespace emscripten;
    printf("Allocating %d bytes for %s\n", nbytes, id.c_str());
    auto parg = val::module_property("parg");
    g_buffer = parg_buffer_alloc(nbytes, PARG_CPU);
    return (int) parg_buffer_lock(g_buffer, PARG_WRITE);
}

static void commit(std::string id)
{
    parg_buffer_unlock(g_buffer);
    parg_asset_onload(id.c_str(), g_buffer);
}

void parg_asset_preload(parg_token id)
{
    using namespace emscripten;
    auto parg = val::module_property("parg");
    std::string name(parg_token_to_string(id));
    parg.call<void>("asset_preload", name);
}

void parg_window_send(const char* msg, double* values, int nvalues)
{
    using namespace emscripten;
    auto parg = val::module_property("parg");
    uint32_t ptr = ((uint32_t) values) / sizeof(double);
    parg.call<void>("onpod", std::string(msg), ptr, nvalues);
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
    parg_asset_set_baseurl("parg/");
}
