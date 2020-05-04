#include "context.h"

#include "error.h"

nafy::context::context(int width, int height, const char *title):
    window(glfwCreateWindow(width, height, title, NULL, NULL)),
    run(false) {

    if (window == NULL) {
        throw error("nafy::context::context(int, int, const char *): Failed to create GLFW window");
    }
}

nafy::context::~context() {
    glfwDestroyWindow(window);
}

void nafy::context::start(scene &source) {
    run = true;
    glfwMakeContextCurrent(window);
    const char *errMsg;
    if (glfwGetError(&errMsg) != GLFW_NO_ERROR) {
        throw error(errMsg);
    }
    scene *current = &source;
    while (run && !glfwWindowShouldClose(window)) {
        current->run(this, &current);
    }
}

void nafy::context::stop() {
    run = false;
}

void nafy::context::changeTitle(const char *title) {
    glfwSetWindowTitle(window, title);
}

