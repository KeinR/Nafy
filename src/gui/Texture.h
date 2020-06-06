#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

namespace nafy {
    class Texture {
    public:
        struct tparam {
            int xWrap;
            int yWrap;

            int minFilter;
            int maxFilter;
        };
    private:
        unsigned int buffer;

        void init(const tparam &param);
        void deInit();
        void steal(Texture &other);
    public:
        // Takes default tparam
        Texture();
        Texture(const tparam &ftl);
        ~Texture();
        Texture(Texture &&other);
        Texture &operator=(Texture &&other);
        void bind();
        void setData(int format, unsigned int width, unsigned int height, const unsigned char *data);

        // void reInit(const tparam &ftl); // Don't do this unless you really messed up
    };
};

#endif
