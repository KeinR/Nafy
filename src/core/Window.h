#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "glfw.h"

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
