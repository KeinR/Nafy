#include "VArray.h"

#include <iostream> // DEBUG

#include "../core/glfw.h"

nafy::VArray::VArray() {
    glGenVertexArrays(1, &handle);
}
nafy::VArray::VArray(unsigned int id): handle(id) {
}
nafy::VArray::VArray(VArray &&other) {
    steal(other);
}
nafy::VArray::~VArray() {
    glDeleteVertexArrays(1, &handle);
}
nafy::VArray &nafy::VArray::operator=(VArray &&other) {
    steal(other);
    return *this;
}

void nafy::VArray::steal(VArray &other) {
    handle = other.handle;
    other.handle = 0;
}

void nafy::VArray::bind() const {
    glBindVertexArray(handle);
}
void nafy::VArray::setParam(unsigned int index, int size, std::size_t stride, const void *offset) {
    glGetError();
    bind();
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
    if (glGetError() != GL_NO_ERROR) {
        std::cout << "DETECTED" << std::endl;
        exit(22);
    }
}
