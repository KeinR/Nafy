#ifndef ENV_H_INCLUDED
#define ENV_H_INCLUDED

#include <string>

#include "glfw.h"

namespace nafy {
    void init();
    void deInit();
    void setCallPath(const char *path);
    std::string getPath(const std::string &path);
    GLFWwindow *getWindow();
    void setWindow(GLFWwindow *window);
}

#endif
