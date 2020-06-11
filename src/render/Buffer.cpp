#include "Buffer.h"

#include <iostream> // DEBUG
#include "../env/env.h" // DEBUG

#include "../core/glfw.h"

nafy::Buffer::Buffer() {
    init();
}

nafy::Buffer::Buffer(unsigned int vertices, unsigned int indices, unsigned int countIndices):
    vertices(vertices), indices(indices), countIndices(countIndices) {
}

nafy::Buffer::~Buffer() {
    deInit();
}

nafy::Buffer::Buffer(Buffer &&other) {
    steal(other);
}
nafy::Buffer::Buffer(const Buffer &other) {
    copy(other);
}
nafy::Buffer &nafy::Buffer::operator=(Buffer &&other) {
    deInit();
    steal(other);
    return *this;
}
nafy::Buffer &nafy::Buffer::operator=(const Buffer &other) {
    copy(other);
    return *this;
}

void nafy::Buffer::steal(Buffer &other) {
    vertices = other.vertices;
    indices = other.indices;
    countIndices = other.countIndices;
    other.vertices = 0;
    other.indices = 0;
    other.countIndices = 0;
}

void nafy::Buffer::copy(const Buffer &other) {
    // Vertex buffer
    int vertSize;
    other.bindVert();
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vertSize);

    const int verticiesLen = vertSize / sizeof(float);
    float *verticiesData = new float[verticiesLen];
    setVerticies(verticiesLen, verticiesData);
    delete[] verticiesData;

    glBindBuffer(GL_COPY_READ_BUFFER, other.vertices);
    glBindBuffer(GL_COPY_WRITE_BUFFER, vertices);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vertSize);

    // Element buffer
    int elemSize;
    other.bindElem();
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &elemSize);

    const int indicesLen = elemSize / sizeof(unsigned int);
    unsigned int *indicesData = new unsigned int[indicesLen];
    setIndices(indicesLen, indicesData);
    delete[] indicesData;

    glBindBuffer(GL_COPY_READ_BUFFER, other.indices);
    glBindBuffer(GL_COPY_WRITE_BUFFER, indices);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, elemSize);

    countIndices = other.countIndices;
}

void nafy::Buffer::init() {
    glGenBuffers(1, &vertices);
    glGenBuffers(1, &indices);
    countIndices = 0;
}

void nafy::Buffer::deInit() {
    glDeleteBuffers(1, &vertices);
    glDeleteBuffers(1, &indices);
    countIndices = 0;
}

void nafy::Buffer::bindVert() const {
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
}
void nafy::Buffer::bindElem() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
}
void nafy::Buffer::bind() const {
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
