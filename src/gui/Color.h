#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

namespace nafy {
    class Color {
    public:
        // Type used for hexidecimal values
        // Note that RGB color values are 24 bit (3 byte),
        // so any superfluous values are ignored.
        typedef unsigned int hex_t;
        // Type used for full RGBA values (0-255) incl.
        typedef unsigned char value_t;
        // Type used for RGBA proportions (0-1) incl.
        typedef float prop_t;
    private:
        // RGBA, Red Green Blue Alpha, in that order
        prop_t color[4];
    public:
        // Yoinked from wikipedia
        // https://en.wikipedia.org/wiki/Web_colors#X11_color_names
        // Predefined hex values to help streamline early development
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

        // Default initialization
        // 0, 0, 0, 255 by default; ie, "opaque black"
        Color();
        // Init with hex and optional alpha
        Color(hex_t hex, value_t alpha = 0xFF);
        // Init with proportions and optional alpha
        Color(prop_t r, prop_t g, prop_t b, prop_t a = 1);

        // Convinience operator, calls setHex(hex)
        Color &operator=(hex_t hex);

        // Hex value for RGB, with first 8 bytes ignored
        void setHex(hex_t hex);
        // Same as above, but including an additional alpha value
        void setHex(hex_t hex, value_t alpha);

        // Color values represented in a conventional 0-255
        void setVal(value_t red, value_t green, value_t blue);
        // Same as above, but including an additional alpha value
        void setVal(value_t red, value_t green, value_t blue, value_t alpha);
        // Same value type as the last two, except set only the alpha
        void setValAlpha(value_t alpha);

        // Proportions, 0-1 representing a percent% of 255
        void setProp(prop_t red, prop_t green, prop_t blue);
        // Same as above, but including an additional alpha value
        void setProp(prop_t red, prop_t green, prop_t blue, prop_t alpha);
        // Same value type as the last two, except set only the alpha
        void setPropAlpha(prop_t alpha);

        // An alternative to using the assignment operator (=)
        void set(const Color &other);

        // Get RGBA data in conventional 0-255 format
        // Any parameter as `nullptr` is ignored
        void getVal(value_t *red, value_t *green, value_t *blue, value_t *alpha) const;
        // Get RGB (no alpha) as a hexidecimal value (represented in base 10 obv.)
        hex_t getHex() const;

        // Get pointer to data, array of length 4 in RGBA format
        // Note that this data is tied to the color object, and will destruct when it does.
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
