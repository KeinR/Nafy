#include "env.h"

#include <iostream>
#include <fstream>

// #include <stb/stb_image.h>

#include "defs.h"
#include "../core/error.h"
#include "../audio/oal.h"

// Location of executable relative to the call dir
// So, if foo/fa/ding.exe was called from a terminal opened in
// foo/, then the call path would be fa/
static std::string homeDir;
// The number of GLFW windows open. When this number goes from 0
// to 1, glfw is initialzied with init(...). When it goes from 1 to
// 0, glfw is deinitialized with deInit()
static int contextCount = 0; // Enlarge data type for more possible instances
// Flag to ensure that glfw is really initialzied
static bool isInit = false;
// The current nafy context
static nafy::Context *currentContext = nullptr;
// glfw cursors
static GLFWcursor *cursor_hand = nullptr;

// A window and context associated together.
// The `parent` will called for all of the `window`'s
// input events
struct call {
    GLFWwindow *window;
    nafy::Context *parent;
};

static std::vector<call> callbacks;

// Private functions
// Terminate glfw
static void deInit();
// Initialize glfw and set OpenGL settings
static GLFWwindow *init(int width, int height, const char *title);
// Initialze window settings
static void initWindow(GLFWwindow *window);
// Due to the nature of glfw, it only accepts global function pointers or
// captureless lambdas... Therefore, all events are processed by these, and each time
// we must iterate through the `callbacks` vector to find a window and it's associated
// context to dispatch the events properly
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
static void keyActionCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWwindow *init(int width, int height, const char *title) {
    glfwInit();
    // Default window hints
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, MSSA);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // A glfw window is required to properly initialize GLAD
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

    // glfw cursor objects
    cursor_hand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    // OpenGL settings
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    isInit = true;

    return window;
}

void deInit() {
    isInit = false;
    glfwTerminate();
    // glfwTerminate "destroys all remaining windows and cursors"
    // https://www.glfw.org/docs/3.1/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901
    cursor_hand = nullptr;
}

void initWindow(GLFWwindow *window) {
    // Set the window's callbacks to the global ones
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
    // Could use contextCount to check if initialized, but I just... Don't feel safe doing that.
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
    // Remove the executable name from the path
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
