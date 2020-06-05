#include "Window.h"

#include "../env/env.h"

nafy::Window::Window(int width, int height, const char *title) {
    handle = plusContext(width, height, title);
}

nafy::Window::~Window() {
    minusContext(handle);
}

GLFWwindow *nafy::Window::get() {
    return handle;
}
