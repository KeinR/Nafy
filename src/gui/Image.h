#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <string>
#include <memory>

#include "../shaders/Shader.h"
#include "../render/Buffer.h"
#include "../render/VArray.h"
#include "../render/Texture.h"
#include "../render/Model.h"

#include "renderable.h"

namespace nafy {

    // Returns 0 if unsupported format
    int getImageFormat(int channels);

    struct imageData {
        int format;
        int width;
        int height;
        unsigned char *data;
    };

    typedef std::shared_ptr<imageData> man_image;

    // Throws an instance of `error` if failed to load image,
    // Either because of an i/o problem, or the channel # is unsupported
    // ALL images from loadImage MUST be deallocated with freeImage
    unsigned char *loadImagePath(const std::string &path, int &width, int &height, int &format);
    man_image loadImageStr(const std::string &path);
    void freeImage(unsigned char *data);

    class Image: public renderable {
        VArray array;
        Buffer buffer;
        Texture texture;
        Model model;
        shader_t shader;

        void init(const shader_t &initShader);
        void initBuffer();
    public:
        Image();
        Image(const Texture::tparam &texParams);
        Image(const shader_t &shader);
        Image(const Texture::tparam &texParams, const shader_t &shader);

        Image(Image &&other) = default;
        Image &operator=(Image &&other) = default;
        Image &operator=(const Image &other);

        // Throws an instance of `error` if failed to load image,
        // Either because of an i/o problem, or the channel # is unsupported
        void loadImage(const std::string &path);
        void setImage(const imageData &image);
        void setImage(const man_image &image);
        void setImage(int format, int width, int height, unsigned char *data);

        void bindShader(const shader_t &shader);
        shader_t getShader();

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
};

#endif
