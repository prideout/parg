#include <parwin.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

static float _pixscale = 1.0f;
static int _winwidth = 0;
static int _winheight = 0;
static int _argc = 0;
static char** _argv = 0;
static par_window_fn_init _init = 0;
static par_window_fn_tick _tick = 0;
static par_window_fn_draw _draw = 0;
static par_window_fn_exit _dispose = 0;

void par_window_setargs(int argc, char* argv[])
{
    _argc = argc;
    _argv = argv;
}

void par_window_oninit(par_window_fn_init fn) { _init = fn; }

void par_window_ontick(par_window_fn_tick fn) { _tick = fn; }

void par_window_ondraw(par_window_fn_draw fn) { _draw = fn; }

void par_window_onexit(par_window_fn_exit fn) { _dispose = fn; }

static void onerror(int error, const char* description)
{
    fputs(description, stderr);
}

static void onkey(GLFWwindow* window, int key, int scancode, int action, int m)
{
    if ((key == 'Q' || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // keypress(key);
    }
}

// GLFW has the origin at top-left
static void onmove(GLFWwindow* window, double x, double y)
{
    // move
}

static void onclick(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        // touchdown
    }
    if (action == GLFW_RELEASE) {
        // touchup
    }
}

static void onscroll(GLFWwindow* window, double dx, double dy)
{
    // scroll
}

int par_window_exec(float winwidth, float winheight, int vsync)
{
    GLFWwindow* window;
    glfwSetErrorCallback(onerror);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // We use Desktop OpenGL 2.1 context only because it's the closest API to
    // OpenGL ES 2.0 that's available on Apple machines.

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // 1.85 is the "Letterbox" aspect ratio, popular in the film industry.
    // Also, the window is small enough to fit just fine on my 13" Pro.

    window = glfwCreateWindow(winwidth, winheight, "par", 0, 0);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwGetWindowSize(window, &_winwidth, &_winheight);
    _pixscale = (float) width / _winwidth;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(vsync);
    if (_init) {
        _init(_winwidth, _winheight, _pixscale);
    }
    glfwMakeContextCurrent(0);
    glfwSetKeyCallback(window, onkey);
    glfwSetCursorPosCallback(window, onmove);
    glfwSetMouseButtonCallback(window, onclick);
    glfwSetScrollCallback(window, onscroll);

    while (!glfwWindowShouldClose(window)) {
        int width, height;

        // Check if the window has been resized.
        glfwGetFramebufferSize(window, &width, &height);
        glfwGetWindowSize(window, &_winwidth, &_winheight);
        _pixscale = (float) width / _winwidth;
        if (_tick) {
            _tick(_winwidth, _winheight, _pixscale, 0);
        }

        // Perform all OpenGL work.
        glfwMakeContextCurrent(window);
        if (_draw && _draw()) {
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                puts("OpenGL Error");
            }
            glfwSwapBuffers(window);
        }
        glfwMakeContextCurrent(0);
        glfwPollEvents();
    }

    // First perform OpenGL-related cleanup.
    glfwMakeContextCurrent(window);
    if (_dispose) {
        _dispose();
    }
    glfwMakeContextCurrent(0);

    // Perform all other cleanup.
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
