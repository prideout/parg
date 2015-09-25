#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

static const int ENABLE_VSYNC = 1;
static const float ASPECT = 1.85;
static const int HEIGHT = 500;

static float pixel_scale = 1.0f;
static int win_width;
static int win_height;

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

#if 0

int main(int argc, char *argv[])
{
    par_window_create(ASPECT * HEIGHT, HEIGHT, VSYNC, argc, argv);
    par_window_oninit(...);
    par_window_ontick(...);
    par_window_ondraw(...);
    par_window_onexit(...);
    return par_window_exec();
}

#endif

int main(int argc, char *argv[])
{
    GLFWwindow* window;
    glfwSetErrorCallback(onerror);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // 1.85 is the "Letterbox" aspect ratio, popular in the film industry.
    // Also, the window is small enough to fit just fine on my 13" Pro.

    window = glfwCreateWindow(ASPECT * HEIGHT, HEIGHT, "par", 0, 0);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwGetWindowSize(window, &win_width, &win_height);
    pixel_scale = (float) width / win_width;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(ENABLE_VSYNC ? 1 : 0);
    // init(win_width, win_height, pixel_scale)
    glfwMakeContextCurrent(0);

    glfwSetKeyCallback(window, onkey);
    glfwSetCursorPosCallback(window, onmove);
    glfwSetMouseButtonCallback(window, onclick);
    glfwSetScrollCallback(window, onscroll);

    while (!glfwWindowShouldClose(window)) {

        // Check if the window has been resized.
        glfwGetFramebufferSize(window, &width, &height);
        glfwGetWindowSize(window, &win_width, &win_height);
        pixel_scale = (float) width / win_width;

        // update(win_width, win_height, pixel_scale);

        glfwMakeContextCurrent(window);
        // if (draw()) {
        //     glfwSwapBuffers(window);
        // }
        glfwMakeContextCurrent(0);

        // Sleep to prevent burning battery.
        if (ENABLE_VSYNC) {
            // TODO
        }

        glfwPollEvents();
    }

    glfwMakeContextCurrent(window);
    // dispose();
    glfwMakeContextCurrent(0);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
