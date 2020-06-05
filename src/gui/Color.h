#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

// TODO: Color constants

namespace nafy {
    class Color {
        // rgba
        float color[4];
    public:
        // 0, 0, 0, 1 by default; ie, opaque black
        Color();
        Color(float r, float b, float g, float a);

        // Hex value for RGBA, with first 8 bytes ignored,
        // next 8 red, then green, blue, and lastly, alpha
        // If the first 8 bits are empty, then the format is assumed to
        // be RGB, and those bits are ignored and the alpha set to 0xFF
        // To illustrate:
        // 0xFF32AC33 -> RGBA -> R=FF, G=32, B=AC, A=33
        // 0xFF32AC == 0x00FF32AC -> Only 3 bytes -> RGB -> R=FF, G=32, B=AC, A=FF
        void setHex(unsigned int hex);
        // Color 0-255
        void setVal(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        // Proportions, 0-1 representing percent of 255
        void setProp(float red, float green, float blue, float alpha = 1.0f);

        // Any parameter as `nullptr` is ignored
        void getVal(unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *alpha);

        // Get pointer to pointer data, array of length 4
        float *get();
    };
}

#endif
