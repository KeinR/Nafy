#include "Image.h"

#include <iostream> // DEBUG

#include <stb/stb_image.h>

#include "../core/glfw.h"
#include "../env/env.h"
#include "../env/defs.h"

int nafy::getImageFormat(int channels) {
    switch (channels) {
        case 4: return GL_RGBA;
        case 3: return GL_RGB;
        case 2: return GL_RG;
        case 1: return GL_RED;
        default: return 0;
    }
}

nafy::Image::Image() {
    init(getContext()->getDefaultSpriteShader());
}
nafy::Image::Image(const Texture::tparam &texParams): texture(texParams) {
    init(getContext()->getDefaultSpriteShader());
}
nafy::Image::Image(shader_t shader) {
    init(shader);
}
nafy::Image::Image(const Texture::tparam &texParams, shader_t shader): texture(texParams) {
    init(shader);
}

void nafy::Image::init(shader_t initShader) {
    bindShader(initShader);
    initBuffer();
}

void nafy::Image::initBuffer() {

    float vertices[16] = {
        // positions   // texture coords
        1.0,   1.0,  1.0f, 1.0f, // top right
        1.0,  -1.0,  1.0f, 0.0f, // bottom right
        -1.0, -1.0,  0.0f, 0.0f, // bottom left
        -1.0,  1.0,  0.0f, 1.0f  // top left 
    };

    unsigned int indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };


    buffer.setVerticies(16, vertices);
    buffer.setIndices(6, indices);

    buffer.bindArr();

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, SHADER_TEXT_SAMPLER_NAME), 0);
}

void nafy::Image::loadImage(const std::string &path) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data == NULL) {
        throw error("Failed to load image");
    }

    const int format = getImageFormat(channels);
    if (!format) {
        stbi_image_free(data);
        throw error("Image has bad number of channels; getImageFormat() returns 0");
    }

    std::cout << "Channels = " << channels << std::endl;

    texture.setData(format, width, height, data);

    stbi_image_free(data);
}

void nafy::Image::setImage(int format, unsigned int width, unsigned int height, const unsigned char *imageData) {
    texture.setData(format, width, height, imageData);
}

void nafy::Image::bindShader(shader_t shader) {
    this->shader = shader;
    model.bindShader(shader);
}

void nafy::Image::setX(int x) {
    model.x = x;
}
void nafy::Image::setY(int y) {
    model.y = y;
}
void nafy::Image::setWidth(unsigned int width) {
    model.width = width;
}
void nafy::Image::setHeight(unsigned int height) {
    model.height = height;
}

int nafy::Image::getX() {
    return model.x;
}
int nafy::Image::getY() {
    return model.y;
}
unsigned int nafy::Image::getWidth() {
    return model.width;
}
unsigned int nafy::Image::getHeight() {
    return model.height;
}

nafy::Texture &nafy::Image::getTexure() {
    return texture;
}

void nafy::Image::render() {
    glUseProgram(shader);
    model.set();
    texture.bind();
    buffer.render();
}