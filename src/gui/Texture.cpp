#include "Texture.h"

#include "../core/glfw.h"

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

nafy::Texture::Texture(Texture &&other) {
    steal(other);
}
nafy::Texture &nafy::Texture::operator=(Texture &&other) {
    steal(other);
    return *this;
}

void nafy::Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, buffer);
}

void nafy::Texture::setData(int format, unsigned int width, unsigned int height, const unsigned char *data) {
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}

void nafy::Texture::setDefaultParams() {
    setParams(defaultParams);
}
void nafy::Texture::setParams(const tparam &params) {
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.xWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.yWrap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.maxFilter);
}
