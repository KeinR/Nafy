#include "env.h"

#include <iostream>
#include <fstream>

// #include <stb/stb_image.h>

#include "defs.h"
#include "../core/error.h"
#include "../audio/oal.h"

static std::string homeDir;
static int contextCount = 0; // Enlarge data type for more possible instances
static bool isInit = false;
static nafy::Context *currentContext = nullptr;
static GLFWcursor *cursor_hand = nullptr;

struct call {
    GLFWwindow *window;
    nafy::Context *parent;
};

static std::vector<call> callbacks;

static void deInit();
static GLFWwindow *init(int width, int height, const char *title);
static void initWindow(GLFWwindow *window);
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
static void keyActionCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWwindow *init(int width, int height, const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, MSSA);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        deInit();
        throw nafy::error("Failed to make GLFW window");
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        deInit();
        throw nafy::error("Failed to initialize GLAD");
    }

    cursor_hand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    isInit = true;

    return window;
}

void deInit() {
    isInit = false;
    glfwTerminate();
    cursor_hand = nullptr;
}

void initWindow(GLFWwindow *window) {
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetKeyCallback(window, keyActionCallback);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    for (call &c : callbacks) {
        if (c.window == window) {
            c.parent->mouseButtonCallback(button, action, mods);
            break;
        }
    }
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    for (call &c : callbacks) {
        if (c.window == window) {
            c.parent->mousePosCallback(xpos, ypos);
            break;
        }
    }
}

void keyActionCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    for (call &c : callbacks) {
        if (c.window == window) {
            c.parent->keyActionCallback(key, scancode, action, mods);
            break;
        }
    }
}

GLFWwindow *nafy::plusContext(int width, int height, const char *title) {
    contextCount++;
    // We're assuming that the hints will stay in place - which they should...
    GLFWwindow *window = isInit ? glfwCreateWindow(width, height, title, NULL, NULL) : init(width, height, title);
    initWindow(window);
    return window;
}

void nafy::minusContext(GLFWwindow *window) {
    contextCount--;
    glfwDestroyWindow(window);
    if (contextCount <= 0) {
        deInit();
        contextCount = 0;
    }
}

void nafy::setContext(Context *ctx) {
    currentContext = ctx;
}

nafy::Context *nafy::getContext() {
    return currentContext;
}

void nafy::registerCallbacks(GLFWwindow *window, Context *ctx) {
    callbacks.push_back({window, ctx});
}

void nafy::deleteCallbacks(Context *ctx) {
    for (std::vector<call>::iterator it = callbacks.begin(); it != callbacks.end(); ++it) {
        if (it->parent == ctx) {
            callbacks.erase(it);
            break;
        }
    }
}

void nafy::setCallPath(const char *path) {
    homeDir = path;
    for (std::string::size_type i = homeDir.length()-1; i >= 0; i--) {
        if (homeDir[i] == '/') {
            i++;
            if (i < homeDir.length()) {
                homeDir.erase(i);
            }
            break;
        }
    }
}

std::string nafy::getPath(const std::string &path) {
    return homeDir + path;
}

void nafy::getWindowSize(int *width, int *height) {
    glfwGetWindowSize(glfwGetCurrentContext(), width, height);
}

float nafy::normX(float x) {
    int width;
    getWindowSize(&width, NULL);
    return x / width * 2 - 1;
}

float nafy::normY(float y) {
    int height;
    getWindowSize(NULL, &height);
    return (y / height * 2 - 1) * -1;
}

bool nafy::loadFile(const std::string &path, char **output, int &length) {
    std::ifstream file(path);
    if (!file.good()) {
        std::cerr << "ERROR: Failed to open file \"" << path << "\"" << std::endl;
        file.close();
        return false;
    }
    file.seekg(0, file.end);
    const int len = file.tellg();
    file.seekg(0, file.beg);

    length = len;
    *output = new char[len];

    file.read(*output, len);

    if (!file.good()) {
        std::cerr << "ERROR: Failed to read from file \"" << path << "\"" << std::endl;
        file.close();
        delete[] *output;
        length = 0;
        return false;
    }
    file.close();
    if (!file.good()) { // Not a terribly big deal: we got the data, all is good
        std::cerr << "WARNING: Failed to close file \"" << path << "\"" << std::endl;
    }

    return true;
}


/*

void nafy::resizeWindow(int width, int height) {
    glfwSetWindowSize(window, width, height);
}

void nafy::renameWindow(const char *title) {
    glfwSetWindowTitle(window, title);
}

bool nafy::setWindowIcon(const char *path) {
    int width, height, channels;
    unsigned char *pixels = stbi_load(path, &width, &height, &channels, 0);
    if (pixels == NULL) {
        return false;
    }

    if (channels == 4) {
        setWindowIcon(width, height, pixels);
    } else { // Correct for bad formats
        const int length = width * height * 4;
        unsigned char *corrected = new unsigned char[length];
        switch (channels) {
            case 3: { // RGB, a classic
                for (int i = 0; i < length;) {
                    const int ind = i - i / 4;
                    corrected[i++] = pixels[ind];
                    corrected[i++] = pixels[ind+1];
                    corrected[i++] = pixels[ind+2];
                    corrected[i++] = 0xFF;
                }
                break;
            }
            case 2: { // Grayscale - alpha; alright...
                for (int i = 0; i < length;) {
                    const int ind = i / 2;
                    corrected[i++] = pixels[ind];
                    corrected[i++] = pixels[ind];
                    corrected[i++] = pixels[ind];
                    corrected[i++] = pixels[ind+1];
                }
                break;
            }
            case 1: { // Grayscale - straightforward enough
                for (int i = 0; i < length; i++) {
                    corrected[i] = pixels[i / 4];
                }
                break;
            }
            default: { // Prop corrupt or smtn
                delete[] corrected;
                stbi_image_free(pixels);
                return false;
            }
        }
        setWindowIcon(width, height, corrected);
        delete[] corrected;
    }
    stbi_image_free(pixels);
    return true;
}

void nafy::setWindowIcon(int width, int height, unsigned char *pixels) {
    GLFWimage image{width, height, pixels};
    glfwSetWindowIcon(window, 1, &image);
}

*/

const char *nafy::getGLErrorStr(GLenum error) {
    switch (error) {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default: return "-Unknown error-";
    }
}

const char *nafy::getALErrorStr(int error) {
    switch (error) {
        case AL_NO_ERROR: return "AL_NO_ERROR";
        case AL_INVALID_NAME: return "AL_INVALID_NAME";
        case AL_INVALID_ENUM: return "AL_INVALID_ENUM";
        case AL_INVALID_VALUE: return "AL_INVALID_VALUE";
        case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION";
        case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY";
        default: return "-Unknown error-";
    }
}

void nafy::setCursorType(cursorType type) {
    switch (type) {
        case HAND:
            glfwSetCursor(glfwGetCurrentContext(), cursor_hand);
            break;
        case DEFAULT:
            glfwSetCursor(glfwGetCurrentContext(), NULL);
            break;
    }
}

static int mouseLock = 0;
void nafy::setCursorHand() {
    if (mouseLock == 0) {
        glfwSetCursor(glfwGetCurrentContext(), cursor_hand);
    }
    mouseLock++;
}
void nafy::releaseCursor() {
    mouseLock--;
    if (mouseLock <= 0) {
        mouseLock = 0;
        glfwSetCursor(glfwGetCurrentContext(), NULL);
    }
}
