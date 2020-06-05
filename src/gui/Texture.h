#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

// TODO: Memory management

#include "../core/glfw.h"

namespace nafy {
    class Texture {
        unsigned int buffer;
        inline void steal(Texture &other);
    public:
        Texture();
        ~Texture();
        Texture(Texture &&other);
        Texture &operator=(Texture &&other);
        void bind();
        void setData(int format, unsigned int width, unsigned int height, const unsigned char *data);
    };
};

#endif
