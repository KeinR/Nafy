#include "env.h"

#include "defs.h"
#include "error.h"

#include <iostream>

static std::string homeDir;
static GLFWwindow *mainWindow;

void nafy::init() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, MSSA);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    mainWindow = glfwCreateWindow(400, 400, "title", NULL, NULL);
    if (mainWindow == NULL) {
        deInit();
        throw error("Failed to make GLFW window");
    }
    glfwMakeContextCurrent(mainWindow);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        deInit();
        throw error("Failed to initialize GLAD");
    }
}

void nafy::deInit() {
    glfwTerminate();
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

GLFWwindow *window() {
    return mainWindow;
}
