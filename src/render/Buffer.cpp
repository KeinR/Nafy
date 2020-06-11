#include "Buffer.h"

#include <iostream> // DEBUG
#include "../env/env.h" // DEBUG

#include "../core/glfw.h"

nafy::Buffer::Buffer(initfunc_t initFunc) {
    setInit(initFunc);
    gen();
    init();
}

nafy::Buffer::~Buffer() {
    deGen();
}

nafy::Buffer::Buffer(Buffer &&other) {
    steal(other);
}
nafy::Buffer::Buffer(const Buffer &other) {
    glGenBuffers(1, &vertices);
    glGenBuffers(1, &indices);
    copy(other);
}
nafy::Buffer &nafy::Buffer::operator=(Buffer &&other) {
    deGen();
    steal(other);
    return *this;
}
nafy::Buffer &nafy::Buffer::operator=(const Buffer &other) {
    glDeleteVertexArrays(1, &array);
    copy(other);
    return *this;
}

void nafy::Buffer::steal(Buffer &other) {
    array = other.array;
    vertices = other.vertices;
    indices = other.indices;
    countIndices = other.countIndices;
    other.array = 0;
    other.vertices = 0;
    other.indices = 0;
    other.countIndices = 0;
}

void nafy::Buffer::copy(const Buffer &other) {
    glGenVertexArrays(1, &array);
    std::cout << "copy" << std::endl;
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
    initFunc = other.initFunc;

    init();
}

void nafy::Buffer::gen() {
    glGenVertexArrays(1, &array);
    glGenBuffers(1, &vertices);
    glGenBuffers(1, &indices);
    countIndices = 0;
}

void nafy::Buffer::deGen() {
    if (array) {
        glDeleteVertexArrays(1, &array);
        glDeleteBuffers(1, &vertices);
        glDeleteBuffers(1, &indices);
    }
    countIndices = 0;
}

void nafy::Buffer::setInit(initfunc_t initFunc) {
    this->initFunc = initFunc;
}

void nafy::Buffer::init() {
    bind();
    if (initFunc != nullptr) {
        initFunc(*this);
    }
}

void nafy::Buffer::bindArray() const {
    glBindVertexArray(array);
}
void nafy::Buffer::bindVert() const {
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
}
void nafy::Buffer::bindElem() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
}
void nafy::Buffer::bind() const {
    bindArray();
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
void nafy::Buffer::setParam(unsigned int index, int size, std::size_t stride, const void *offset) {
    bind();
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
}

void nafy::Buffer::render() {
    bind();
    glDrawElements(GL_TRIANGLES, countIndices, GL_UNSIGNED_INT, 0);
}
