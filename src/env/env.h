#ifndef ENV_H_INCLUDED
#define ENV_H_INCLUDED

#include <string>

#include "../core/glfw.h"

#include "../core/Context.h"

/*
* Env, syncs operations across all of Nafy
*/

// TODO: break this up

namespace nafy {
    // Create a new GLFWwindow
    GLFWwindow *plusContext(int width, int height, const char *title);
    // delete a GLFWwindow
    void minusContext(GLFWwindow *window);
    // Set a context as current globally
    void setContext(Context *ctx);
    // Get current context
    Context *getContext();

    // Get the current context window's width and height - needed
    // by some classes (Model) to position accordingly
    void getWindowSize(int *width, int *height);

    // Should be called if you run the program from anywhere
    // other than the executable location
    void setCallPath(const char *path);

    // Get path relative to the location of the executable
    std::string getPath(const std::string &path);
    // Convert screen coords to GL coords
    float normX(float x);
    float normY(float y);
    // Load file as a block of data
    bool loadFile(const std::string &path, char **output, int &length);

    // Get error strings from error codes
    const char *getGLErrorStr(GLenum error);
    const char *getALErrorStr(int error);

    // Different cursor types
    enum cursorType {
        DEFAULT,
        HAND
    };

    // Forces the change
    void setCursorType(cursorType type);

    // TODO: This is a dumb way of doing this
    // Accounts for other requests to do the same; reccomended
    void releaseCursor(); // Back to default
    void setCursorHand(); // Hand
}

#endif
