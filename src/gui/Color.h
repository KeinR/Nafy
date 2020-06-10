#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

// TODO: Color constants

namespace nafy {
    class Color {
    public:
        typedef unsigned int hex_t;
        typedef unsigned char value_t;
        typedef float prop_t;
    private:
        // rgba
        prop_t color[4];
    public:
        // Yoinked from wikipedia
        // https://en.wikipedia.org/wiki/Web_colors#X11_color_names
        static constexpr hex_t red = 0xFF0000;
        static constexpr hex_t green = 0x008000;
        static constexpr hex_t blue = 0x0000FF;
        static constexpr hex_t black = 0x000000;
        static constexpr hex_t white = 0xFFFFFF;
        static constexpr hex_t silver = 0xC0C0C0;
        static constexpr hex_t gray = 0x808080;
        static constexpr hex_t maroon = 0x800000;
        static constexpr hex_t yellow = 0xFFFF00;
        static constexpr hex_t olive = 0x808000;
        static constexpr hex_t lime = 0x00FF00;
        static constexpr hex_t aqua = 0x00FFFF;
        static constexpr hex_t teal = 0x008080;
        static constexpr hex_t navy = 0x000080;
        static constexpr hex_t fuchsia = 0xFF00FF;
        static constexpr hex_t purple = 0x800080;

        // 0, 0, 0, 1 by default; ie, opaque black
        Color();
        Color(hex_t hex, value_t alpha = 0xFF);
        Color(prop_t r, prop_t g, prop_t b, prop_t a = 1);

        // Sets a color to a hex value (no alpha, first byte ignored)
        Color &operator=(hex_t hex);

        // Hex value for RGBA, with first 8 bytes ignored
        void setHex(hex_t hex);
        void setHex(hex_t hex, value_t alpha);
        // Color 0-255
        void setVal(value_t red, value_t green, value_t blue);
        void setVal(value_t red, value_t green, value_t blue, value_t alpha);
        void setValAlpha(value_t alpha);
        // Proportions, 0-1 representing percent of 255
        void setProp(prop_t red, prop_t green, prop_t blue);
        void setProp(prop_t red, prop_t green, prop_t blue, prop_t alpha);
        void setPropAlpha(prop_t alpha);

        void set(const Color &other);

        // Any parameter as `nullptr` is ignored
        void getVal(value_t *red, value_t *green, value_t *blue, value_t *alpha) const;

        // Get pointer to pointer data, array of length 4
        prop_t *get();

        // In `percent`, 1 is 100%
        // Basically for each one does f(x)=x+percent*x,
        // effectively brightening the previous color
        // Returns a new color object with the new values.
        // Negative values for `percent` will instead darken.
        Color brighten(float percent) const;
    };
}

#endif
