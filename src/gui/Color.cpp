#include "Color.h"

nafy::Color::Color() {
    setProp(0.0f, 0.0f, 0.0f, 1.0f);
}
nafy::Color::Color(hex_t hex, value_t alpha) {
    setHex(hex, alpha);
}
nafy::Color::Color(prop_t r, prop_t g, prop_t b, prop_t a) {
    setProp(r, b, g, a);
}

nafy::Color &nafy::Color::operator=(hex_t hex) {
    setHex(hex);
    return *this;
}

void nafy::Color::setHex(hex_t hex) {
    setVal(
        (hex >> 16) & 0xFF,
        (hex >> 8) & 0xFF,
        hex & 0xFF
    );
}
void nafy::Color::setHex(hex_t hex, value_t alpha) {
    setVal(
        (hex >> 16) & 0xFF,
        (hex >> 8) & 0xFF,
        hex & 0xFF,
        alpha
    );
}

void nafy::Color::setVal(value_t red, value_t green, value_t blue) {
    setProp(red / 255.0f, green / 255.0f, blue / 255.0f, color[3]);
}
void nafy::Color::setVal(value_t red, value_t green, value_t blue, value_t alpha) {
    setProp(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}
void nafy::Color::setValAlpha(value_t alpha) {
    setPropAlpha(alpha / 255.0f);
}

void nafy::Color::setProp(prop_t red, prop_t green, prop_t blue) {
    setProp(red, green, blue, color[3]);
}
void nafy::Color::setProp(prop_t red, prop_t green, prop_t blue, prop_t alpha) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;
}
void nafy::Color::setPropAlpha(prop_t alpha) {
    color[3] = alpha;
}

void nafy::Color::set(const Color &other) {
    for (int i = 0; i < 4; i++) {
        color[i] = other.color[i];
    }
}

void nafy::Color::getVal(value_t *red, value_t *green, value_t *blue, value_t *alpha) const {
    #define TRYGET(var, i) if (var != nullptr) *var = color[i] * 0xFF;
    TRYGET(red, 0)
    TRYGET(green, 1)
    TRYGET(blue, 2)
    TRYGET(alpha, 3)
    #undef TRYGET
}

nafy::Color::prop_t *nafy::Color::get() {
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
