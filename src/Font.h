#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <string>
#include <vector>

#include <stb/stb_truetype.h>

namespace nafy {
    struct wrapLine {
        std::vector<int>::size_type end;
        std::vector<int>::size_type nextResume;
        int width;
    };

    struct fontMetrics {
        float scale;
        int ascent;
        int descent;
        int lineGap;
    };

    class Font {
        unsigned char *buffer;
        int spaceGlyph;
        int ascent;
        int descent;
        int lineGap;
        stbtt_fontinfo info;
    public:
        Font(const char *filepath);
        ~Font();
        std::vector<wrapLine> wrapString(const fontMetrics *metrics, const std::vector<int> &str, const int wrapWidth);
        std::vector<int> indexString(const std::string &str);
        int glyphStrPixelWidth(const fontMetrics *metrics, const std::vector<int> &glyphs);
        void render(const fontMetrics *metrics,
                    std::vector<int>::const_iterator it, std::vector<int>::const_iterator endExcl,
                    unsigned char *out, int outWidth);
        fontMetrics getMetrics(int fontSize);
    };
}

#endif
