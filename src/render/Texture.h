#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

namespace nafy {
    class Texture {
    public:
        typedef unsigned char data_t;
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
        void copy(const Texture &other);
    public:
        // Takes default tparam
        Texture();
        Texture(const tparam &params);
        ~Texture();
        Texture(Texture &&other);
        Texture(const Texture &other);
        Texture &operator=(Texture &&other);
        Texture &operator=(const Texture &other);
        void bind() const;
        void setData(int format, unsigned int width, unsigned int height, const data_t *data);

        void setDefaultParams();
        void setParams(const tparam &params);
    };
};

#endif
