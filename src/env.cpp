#include "env.h"

#include <iostream>
#include <fstream>

#include <stb/stb_image.h>

#include "defs.h"
#include "error.h"
#include "shaders.h"

static std::string homeDir;
static int contexts = 0; // Enlarge data type for more possible instances
static bool running = false;

static void deInit();
static GLFWwindow *init(int width, int height, const char *title);

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
        throw error("Failed to make GLFW window");
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        deInit();
        throw error("Failed to initialize GLAD");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    nafy::shaders::init();
    running = true;
    return window;
}

void deInit() {
    running = false;
    nafy::shaders::deInit();
    glfwTerminate();
}

GLFWwindow *nafy::plusContext(int width, int height, const char *title) {
    contexts++;
    // We're assuming that the hints will stay in place - which they should...
    return running ? glfwCreateWindow(width, height, title, NULL, NULL) : init(width, height, title);
}

void nafy::minusContext(GLFWwindow *window) {
    contexts--;
    glfwDestroyWindow(window);
    if (contexts <= 0) {
        deInit();
        contexts = 0;
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
