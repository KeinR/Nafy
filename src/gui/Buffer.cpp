#include "Buffer.h"

#include "../core/glfw.h"

nafy::Buffer::Buffer() {
    init();
}

nafy::Buffer::Buffer(unsigned int array, unsigned int vertices, unsigned int indices):
    array(array), vertices(vertices), indices(indices) {
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
    other.array = 0;
}

void nafy::Buffer::init() {
    glGenVertexArrays(1, &array);
    glGenBuffers(1, &vertices);
    glGenBuffers(1, &indices);
}

void nafy::Buffer::deInit() {
    if (array) {
        glDeleteVertexArrays(1, &array);
        glDeleteTextures(1, &vertices);
        glDeleteTextures(1, &indices);
    }
}

void nafy::Buffer::bindArr() {
    glBindVertexArray(array);
}
void nafy::Buffer::bindVert() {
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
}
void nafy::Buffer::bindElem() {
    glBindBuffer(GL_ARRAY_BUFFER, indices);
}
void nafy::Buffer::bind() {
    bindArr();
    bindVert();
    bindElem();
}

void nafy::Buffer::setVerticies(std::size_t size, float *data) {
    bindVert();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
void nafy::Buffer::setIndices(std::size_t size, unsigned int *data) {
    bindElem();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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

    return other;
}
