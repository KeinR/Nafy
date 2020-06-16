#include "Texture.h"

#include <iostream> // DEBUG
#include <string>

#include "../core/error.h"

#include "../core/glfw.h"

// def'd to be extra safe
#define TEXTURE_TYPE GL_TEXTURE_2D
#define TEXTURE_PIXEL_TYPE GL_UNSIGNED_BYTE

// Default parameters
constexpr nafy::Texture::tparam defaultParams{
    GL_CLAMP_TO_BORDER,
    GL_CLAMP_TO_BORDER,
    GL_NEAREST,
    GL_NEAREST
};

nafy::Texture::Texture() {
    init(defaultParams);
}

nafy::Texture::Texture(const tparam &param) {
    init(param);
}

nafy::Texture::~Texture() {
    deInit();
}

void nafy::Texture::init(const tparam &params) {
    glGenTextures(1, &buffer);

    setParams(params);
}

void nafy::Texture::deInit() {
    glDeleteTextures(1, &buffer);
}

void nafy::Texture::steal(Texture &other) {
    buffer = other.buffer;
    other.buffer = 0;
}

void nafy::Texture::copy(const Texture &other) {
    // Get metadata from other's texture buffer and
    // copy the texture data in other's buffer to this's
    other.bind();
    GLint width, height, format;
    glGetTexLevelParameteriv(TEXTURE_TYPE, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(TEXTURE_TYPE, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(TEXTURE_TYPE, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
    GLint channels;
    switch (format) {
        case GL_RGBA: channels = 4; break;
        case GL_RGB: channels = 3; break;
        case GL_RG: channels = 2; break;
        case GL_RED: channels = 1; break;
        default: throw gl_error("Texture encountered unexpected image format while copying: " + std::to_string(format));
    }
    const GLint length = channels * width * height;
    data_t *copyBuffer = new data_t[length];
    glGetTexImage(TEXTURE_TYPE, 0, format, TEXTURE_PIXEL_TYPE, copyBuffer);
    setData(format, width, height, copyBuffer);
    delete[] copyBuffer;
}

nafy::Texture::Texture(Texture &&other) {
    steal(other);
}
nafy::Texture::Texture(const Texture &other) {
    copy(other);
}
nafy::Texture &nafy::Texture::operator=(Texture &&other) {
    steal(other);
    return *this;
}
nafy::Texture &nafy::Texture::operator=(const Texture &other) {
    copy(other);
    return *this;
}

void nafy::Texture::bind() const {
    glBindTexture(TEXTURE_TYPE, buffer);
}

void nafy::Texture::setData(int format, unsigned int width, unsigned int height, const data_t *data) {
    bind();
    glTexImage2D(TEXTURE_TYPE, 0, format, width, height, 0, format, TEXTURE_PIXEL_TYPE, data);
}

void nafy::Texture::setDefaultParams() {
    setParams(defaultParams);
}
void nafy::Texture::setParams(const tparam &params) {
    bind();

    // x/y wrap parameter
    glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, params.xWrap);
    glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, params.yWrap);

    // min/max filtering parameter
    glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, params.minFilter);
    glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, params.maxFilter);
}
