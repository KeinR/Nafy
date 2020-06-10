#include "Color.h"

nafy::Color::Color(): color{0.0f, 0.0f, 0.0f, 1.0f} {
}
nafy::Color::Color(float r, float b, float g, float a): color{r, g, b, a} {
}

void nafy::Color::setHex(unsigned int hex) {
    const unsigned char r = hex >> 24;
    if (r) {
        setVal(
            r,
            (hex >> 16) & 0xFF,
            (hex >> 8) & 0xFF,
            hex & 0xFF
        );
    } else {
        setVal(
            (hex >> 16) & 0xFF,
            (hex >> 8) & 0xFF,
            hex & 0xFF,
            0xFF
        );
    }
}

void nafy::Color::setVal(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
    setProp(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

void nafy::Color::setProp(float red, float green, float blue, float alpha) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;
}

void nafy::Color::set(const Color &other) {
    for (int i = 0; i < 4; i++) {
        color[i] = other.color[i];
    }
}

void nafy::Color::getVal(unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *alpha) const {
    #define TRYGET(var, i) if (var != nullptr) *var = color[i] * 0xFF;
    TRYGET(red, 0)
    TRYGET(green, 1)
    TRYGET(blue, 2)
    TRYGET(alpha, 3)
    #undef TRYGET
}

float *nafy::Color::get() {
    return color;
}

nafy::Color nafy::Color::brighten(float percent) const {
    Color c;
    for (int i = 0; i < 4; i++) {
        c.color[i] = color[i] + color[i] * percent;
        if (c.color[i] > 1) {
            c.color[i] = 1;
        }
    }
    return c;
}
