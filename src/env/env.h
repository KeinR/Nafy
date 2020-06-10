#ifndef ENV_H_INCLUDED
#define ENV_H_INCLUDED

#include <string>

#include "../core/glfw.h"

#include "../core/Context.h"

namespace nafy {
    GLFWwindow *plusContext(int width, int height, const char *title);
    void minusContext(GLFWwindow *window);
    void setContext(Context *ctx);
    Context *getContext();
    void registerCallbacks(GLFWwindow *window, Context *ctx);
    void deleteCallbacks(Context *ctx);

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

    const char *getGLErrorStr(GLenum error);
    const char *getALErrorStr(int error);

    enum cursorType {
        DEFAULT,
        HAND
    };

    // Forces the change
    void setCursorType(cursorType type);

    // Accounts for other requests to do the same; reccomended
    void releaseCursor(); // Back to default
    void setCursorHand(); // Hand
}

#endif
