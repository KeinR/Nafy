#include "Buffer.h"

#include <iostream> // DEBUG
#include "../env/env.h" // DEBUG

#include "../core/glfw.h"

nafy::Buffer::Buffer() {
    init();
}

nafy::Buffer::Buffer(unsigned int array, unsigned int vertices, unsigned int indices, unsigned int countIndices):
    array(array), vertices(vertices), indices(indices), countIndices(countIndices) {
}

nafy::Buffer::~Buffer() {
    deInit();
}

nafy::Buffer::Buffer(Buffer &&other) {
    steal(other);
}
nafy::Buffer &nafy::Buffer::operator=(Buffer &&other) {
    deInit();
    steal(other);
    return *this;
}

void nafy::Buffer::steal(Buffer &other) {
    array = other.array;
    vertices = other.vertices;
    indices = other.indices;
    countIndices = other.countIndices;
    other.array = 0;
    other.countIndices = 0;
}

void nafy::Buffer::init() {
    glGenVertexArrays(1, &array);
    glGenBuffers(1, &vertices);
    glGenBuffers(1, &indices);
    countIndices = 0;
}

void nafy::Buffer::deInit() {
    if (array) {
        glDeleteVertexArrays(1, &array);
        glDeleteTextures(1, &vertices);
        glDeleteTextures(1, &indices);
    }
    countIndices = 0;
}

void nafy::Buffer::bindArr() {
    glBindVertexArray(array);
}
void nafy::Buffer::bindVert() {
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
}
void nafy::Buffer::bindElem() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
}
void nafy::Buffer::bind() {
    bindArr();
    bindVert();
    bindElem();
}

void nafy::Buffer::setVerticies(int count, float *data) {
    bindVert();
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
}
void nafy::Buffer::setIndices(int count, unsigned int *data) {
    countIndices = count;
    bindElem();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void nafy::Buffer::render() {
    bind();
    glDrawElements(GL_TRIANGLES, countIndices, GL_UNSIGNED_INT, 0);
}

nafy::Buffer nafy::Buffer::derive() {
    Buffer other;

    // Vertex buffer
    int vertSize;
    bindVert();
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vertSize);

    glBindBuffer(GL_COPY_READ_BUFFER, vertices);
    glBindBuffer(GL_COPY_WRITE_BUFFER, other.vertices);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vertSize);

    // Element buffer
    int elemSize;
    bindElem();
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &elemSize);

    glBindBuffer(GL_COPY_READ_BUFFER, indices);
    glBindBuffer(GL_COPY_WRITE_BUFFER, other.indices);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, elemSize);
    
    other.countIndices = countIndices;

    return other;
}
