#ifndef ENV_H_INCLUDED
#define ENV_H_INCLUDED

#include <string>

#include "glfw.h"

#include "context.h"

namespace nafy {
    GLFWwindow *plusContext(int width, int height, const char *title);
    void minusContext(GLFWwindow *window);
    void setContext(context *ctx);
    context *getContext();

    // Should be called if you run the program from anywhere
    // other than the executable location
    void setCallPath(const char *path);

    void getWindowSize(int *width, int *height);

    // Get based off of the current dir
    std::string getPath(const std::string &path);
    // Convert screen coords to GL coords
    float normX(float x);
    float normY(float y);
    bool loadFile(const std::string &path, char **output, int &length);

    bool engineUp();
    GLFWwindow *getWindow();
}

#endif
