#include "Window.h"

nafy::Window::Window(int width, int height, const char *title) {
    plusContext(handle);
}

nafy::Window::~Window() {
    minusContext(handle);
}

GLFWWindow *get() {
    return handle;
}
