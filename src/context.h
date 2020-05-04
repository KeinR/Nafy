#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <string>

#include "glfw.h"

#include "story.h"

namespace nafy {
    class context {
        GLFWwindow *window;
        bool run;
    public:
        context(int width = 400, int height = 400, const char *title = "Nafy");
        ~context();
        void start(scene &source);
        void stop();
        void changeTitle(const char *title);
    };
}

#endif
