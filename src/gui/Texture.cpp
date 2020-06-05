#include "Texture.h"

nafy::Texture::Texture() {
    glGenTextures(1, &buffer);

    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

nafy::Texture::~Texture() {
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
