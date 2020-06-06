#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <string>

#include "../shaders/Shader.h"
#include "../render/Buffer.h"
#include "../render/Texture.h"
#include "../render/Model.h"

#include "renderable.h"

namespace nafy {
    class Image: public renderable {
        Buffer buffer;
        Texture texture;
        Model model;
        shader_t shader;

        void init(shader_t initShader);
        void initBuffer();
    public:
        Image();
        Image(const Texture::tparam &texParams);
        Image(shader_t shader);
        Image(const Texture::tparam &texParams, shader_t shader);

        // Throws an instance of `error` if failed to load image,
        // Either because of an i/o problem, or the channel # is unsupported
        void loadImage(const std::string &path);
        void setImage(int format, unsigned int width, unsigned int height, const unsigned char *imageData);

        void bindShader(shader_t shader);

        void setX(int x);
        void setY(int y);
        void setWidth(unsigned int width);
        void setHeight(unsigned int height);

        int getX();
        int getY();
        unsigned int getWidth();
        unsigned int getHeight();

        Texture &getTexure();

        void render() override;
    };

    // Returns 0 if unsupported format
    int getImageFormat(int channels);
};

#endif
