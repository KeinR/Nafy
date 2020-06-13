#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "glfw.h"

/*
* Wrapps a glfw window, and provides functions for interacting with it
*/

// TODO: Use this for something
// TODO: Memory management

namespace nafy {
    class Window {
        GLFWwindow *handle;
    public:
        Window(int width, int height, const char *title);
        ~Window();
        GLFWwindow *get();
        // void makeCurrent();
    };
}

#endif
