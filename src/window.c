#include <parg.h>
#include <parwin.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "lodepng.h"

static int _argc = 0;
static char** _argv = 0;
static float _touchpt[2] = {0};
static float _pixscale = 1.0f;
static int _winwidth = 0;
static int _winheight = 0;
static parg_window_fn_init _init = 0;
static parg_window_fn_tick _tick = 0;
static parg_window_fn_draw _draw = 0;
static parg_window_fn_exit _dispose = 0;
static parg_window_fn_input _input = 0;
static parg_window_fn_message _message = 0;

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

static void onerror(int error, const char* description)
{
    fputs(description, stderr);
    fputs("\n", stderr);
}

static void onkey(GLFWwindow* window, int key, int scancode, int action, int m)
{
    if ((key == 'Q' || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (_input && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        _input(PARG_EVENT_KEYPRESS, key, 0, 0);
    }
}

// GLFW has the origin at top-left
static void onmove(GLFWwindow* window, double x, double y)
{
    if (_input) {
        _touchpt[0] = x / _winwidth;
        _touchpt[1] = (_winheight - 1 - y) / _winheight;
        _input(PARG_EVENT_MOVE, _touchpt[0], _touchpt[1], 0);
    }
}

static void onclick(GLFWwindow* window, int button, int action, int mods)
{
    if (!_input) {
        return;
    }
    if (action == GLFW_PRESS) {
        _input(PARG_EVENT_DOWN, _touchpt[0], _touchpt[1], 0);
    }
    if (action == GLFW_RELEASE) {
        _input(PARG_EVENT_UP, _touchpt[0], _touchpt[1], 0);
    }
}

static void onscroll(GLFWwindow* window, double dx, double dy)
{
    if (_input) {
        _input(PARG_EVENT_MOVE, _touchpt[0], _touchpt[1], dy);
    }
}

int parg_window_exec(float winwidth, float winheight, int vsync, int aa)
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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    if (aa) {
        glfwWindowHint(GLFW_SAMPLES, 4);
    }

    // This removes borders which looks nice sometimes.
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);

// This GLFW feature doesn't exist yet but it's on the way.
#if GLFW_VERSION_MAJOR > 3 && GLFW_VERSION_MINOR > 1000
    glfwWindowHint(GLFW_ALPHA_MASK, GL_TRUE);
#endif

    char* capture = 0;
    for (int i = 1; i < _argc - 1; i++) {
        if (0 == strcmp(_argv[i], "-capture")) {
            capture = _argv[i + 1];
            glfwWindowHint(GLFW_VISIBLE, 0);
        }
    }

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

    struct timeval tm1;
    gettimeofday(&tm1, NULL);

    while (!glfwWindowShouldClose(window)) {
        int width, height;

        // Get microseconds.
        struct timeval tm2;
        gettimeofday(&tm2, 0);
        unsigned long long milliseconds = 1000 * (tm2.tv_sec - tm1.tv_sec) +
            (tm2.tv_usec - tm1.tv_usec) / 1000;

        // Check if the window has been resized.
        glfwGetFramebufferSize(window, &width, &height);
        glfwGetWindowSize(window, &_winwidth, &_winheight);
        _pixscale = (float) width / _winwidth;
        int needs_draw = 1;
        if (_tick) {
            needs_draw =
                _tick(_winwidth, _winheight, _pixscale, milliseconds / 1000.0);
        }

        // Perform all OpenGL work.
        glfwMakeContextCurrent(window);
        if (needs_draw && _draw) {
            if (capture) {
                parg_framebuffer_create(width, height);
            }
            _draw();
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                puts("OpenGL Error\n");
            }
            glfwSwapBuffers(window);
            if (capture) {
                unsigned char* buffer = malloc(width * height * 4);
                glReadPixels(
                    0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                parg_texture_fliprows(buffer, width * 4, height);
                lodepng_encode32_file(capture, buffer, width, height);
                free(buffer);
                break;
            }
        }
        glfwMakeContextCurrent(0);
        glfwPollEvents();
    }

    // First perform OpenGL-related cleanup.
    glfwMakeContextCurrent(window);
    if (_dispose) {
        _dispose();
    }
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        puts("OpenGL Error");
    }
    glfwMakeContextCurrent(0);

    // Perform all other cleanup.
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
