#ifndef ENV_H_INCLUDED
#define ENV_H_INCLUDED

#include <string>

#include "glfw.h"

namespace nafy {
    void init(int width = 400, int height = 400, const char *title = "Nafy");
    void deInit();
    void setCallPath(const char *path);

    void setFont(const char *path);

    void resizeWindow(int width, int height);
    void renameWindow(const char *title);
    bool setWindowIcon(const char *path);
    // Note that the format MUST be RGBA
    void setWindowIcon(int width, int height, unsigned char *pixels);


    int getWinWidth();
    int getWinHeight();

    // Get based off of the current dir
    std::string getPath(const std::string &path);
    // Convert screen coords to GL coords
    float normX(float x);
    float normY(float y);

    bool engineUp();
    GLFWwindow *getWindow();
}

#endif
