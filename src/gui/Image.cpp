#include "Image.h"

#include <iostream> // DEBUG

#include <stb/stb_image.h>

#include "../core/glfw.h"
#include "../core/Cache.h"
#include "../env/env.h"
#include "../env/defs.h"

namespace cache {

    using namespace nafy;

    static Cache<Buffer> buffer([]()-> Cache<Buffer>::data_t {

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

        std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>([](Buffer &buffer)->void{
            buffer.setParam(0, 2, 4 * sizeof(float), (void*)0);
            buffer.setParam(1, 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        });

        buffer->setVerticies(16, vertices);
        buffer->setIndices(6, indices);

        return buffer;
    });
};


int nafy::getImageFormat(int channels) {
    switch (channels) {
        case 4: return GL_RGBA;
        case 3: return GL_RGB;
        case 2: return GL_RG;
        case 1: return GL_RED;
        default: return 0;
    }
}
unsigned char *nafy::loadImagePath(const std::string &path, int &width, int &height, int &format) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &format, 0);
    if (data == NULL) {
        throw error("Failed to load image");
    }

    format = getImageFormat(format);
    if (!format) {
        stbi_image_free(data);
        throw error("Image has bad number of channels; getImageFormat() returns 0");
    }

    return data;
}
nafy::man_image nafy::loadImageStr(const std::string &path) {

    int width, height, format;
    unsigned char *data = loadImagePath(path, width, height, format);

    return std::shared_ptr<imageData>(new imageData{format, width, height, data}, [](imageData *image) -> void {
        stbi_image_free(image->data);
        delete image;
    });
}
void nafy::freeImage(unsigned char *data) {
    stbi_image_free(data);
}


nafy::Image::Image() {
    init(getContext()->getDefaultSpriteShader());
}
nafy::Image::Image(const Texture::tparam &texParams): texture(texParams) {
    init(getContext()->getDefaultSpriteShader());
}
nafy::Image::Image(const shader_t &shader) {
    init(shader);
}
nafy::Image::Image(const Texture::tparam &texParams, const shader_t &shader): texture(texParams) {
    init(shader);
}

void nafy::Image::init(const shader_t &initShader) {
    bindShader(initShader);
    buffer = cache::buffer.get();
}

void nafy::Image::loadImage(const std::string &path) {
    setImage(loadImageStr(path));
}

void nafy::Image::setImage(const imageData &image) {
    texture.setData(image.format, image.width, image.height, image.data);
}

void nafy::Image::setImage(const man_image &image) {
    texture.setData(image->format, image->width, image->height, image->data);
}

void nafy::Image::setImage(int format, int width, int height, unsigned char *data) {
    texture.setData(format, width, height, data);
}

void nafy::Image::bindShader(const shader_t &shader) {
    this->shader = shader;
    model.bindShader(shader);
    shader->uniSampler0();
}

nafy::shader_t nafy::Image::getShader() {
    return shader;
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
    shader->use();
    model.set();
    texture.bind();
    buffer->render();
}
