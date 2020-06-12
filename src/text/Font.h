#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <string>
#include <vector>
#include <memory>

#include "ftype.h"

// Technically a face, but more people know what a font is, so...
// An instance of a font that can be transformed, resized, etc. and render strings to bitmaps

class Font {
public:
    typedef std::shared_ptr<Font> type;

    typedef unsigned int glyph_type;
    typedef std::vector<glyph_type> glyph_str;
    typedef glyph_str::const_iterator glyph_iterator;
    typedef unsigned int map_size;
    typedef int ofs_type;

    struct line {
        // The start index, inclusive
        glyph_iterator start;
        // The stop index, exlcusive
        glyph_iterator end;

        ofs_type horizontalOffset; // 26.6
        map_size width; // 26.6 (purely for efficiency)
    };

    typedef std::vector<line> line_str;
    typedef line_str::const_iterator line_iterator;

    struct renderConf {
        int channels;
        float *cMod;
        unsigned char *bitmap;
        // sizeof(unsigned char) / channels (the number of pixels) that must be travelled to get to the next line...
        // Often times this is the width
        unsigned int verticalStride;
        map_size lineHeight;

        renderConf();
        renderConf(const int channels, float *cMod, unsigned char *bitmap, unsigned int verticalStride, map_size lineHeight);
    };

    struct vec2 {
        map_size x;
        map_size y;

        vec2();
        vec2(map_size x, map_size y);
    };

    enum textAlign {
        left,
        right,
        center
    };

private:
    FT_Face face;
    std::shared_ptr<unsigned char> data;
    glyph_type space;
    // Switch between the newline defined by the font and our own value,
    // because many fonts will just have newline as a nul
    glyph_type fontNewline;
    struct char_metrics {
        int lsb;
        int kern;
        FT_Pos advance;
    };
    inline glyph_type getGlyph(glyph_type glyph);
    void steal(Font &other);
    void copyPOD(const Font &other);
    void delFace();
    std::vector<char_metrics> measureString(glyph_iterator start, const glyph_iterator &end);
    line consLine(const glyph_iterator &start, const glyph_iterator &end, const ofs_type horizontalOffset, const map_size width);
public:
    static constexpr unsigned int defaultSize = 16;
    Font();
    Font(FT_Face face, const std::shared_ptr<unsigned char> &data);
    Font(Font &&other);
    ~Font();
    Font &operator=(Font &&other);


    /* start public methods */

    // Set the character pixel size
    FT_Error setSize(unsigned int size);

    /* end public methods */

    /* private methods, use only if you know what you're doing */


    FT_Face getFace();

    // Renders with origin at top-left; ie, increasing y is down
    // Returns the resulting bitmap, which must be deallocated with delete[]
    // Width and height are the pixel width and height of the bitmap
    // NOTE: `horizontalOffset` should be in 26.6 fractional pixels
    void renderTo(glyph_iterator start, const glyph_iterator &end, const renderConf &render, ofs_type horizontalOffset);
    void renderGlyphTo(glyph_type glyph, glyph_type next, const renderConf &render, vec2 &pen);
    unsigned char *renderBitmap(const glyph_iterator &start, const glyph_iterator &end, const int channels, float cMod[], map_size &width, map_size &height);
    unsigned char *renderLines(line_iterator start, const line_iterator &end, const int channels, float cMod[], float lineSpacingMod, map_size &width, map_size &height);
    // makes a line from the given range
    line makeLine(const glyph_iterator &start, const glyph_iterator &end);
    // Iterators in the resultant vector point to positions in the source `glyph_iterator`
    // NOT the same for `wrappingWidth`, because you can only really wrap to a whole pixel... Although do note that values over 2^26-1
    // are, erm... discouraged, as `wrappingWidth` is shifted 6 left to be compatable with the in-use 26.6 format
    line_str getLines(const glyph_iterator &start, const glyph_iterator &end, unsigned int wrappingWidth, const textAlign align);
    // NOTE: `horizontalOffset` and `verticalOffset` are in 26.6 fractional pixels
    void stringMetrics(glyph_iterator start, const glyph_iterator &end, map_size *width, map_size *height, ofs_type *horizontalOffset, ofs_type *verticalOffset);
    // Both return value in 26.6 positive or negative
    int stringDescent(glyph_iterator start, const glyph_iterator &end);
    // Both return value in 26.6 positive
    map_size stringAscent(glyph_iterator start, const glyph_iterator &end);
    // Convert character string to glyph string for faster rendering
    glyph_str indexString(std::string::const_iterator start, const std::string::const_iterator &end);
    // Breaks the given lines up into groups, each with a height of less than `maxHeight`
    // Returns a vector of iterators representing the exclusive end of each line group with respect to the given iterators 
    std::vector<line_iterator> breakupLines(line_iterator start, const line_iterator &end, float lineSpacingMod, map_size maxHeight);
    // Where the last one breaks the line range into sections, this one returns an iterator to the last exclusive line, the one that
    // causes the line height to exceed `maxHeight`
    line_iterator truncLines(line_iterator start, const line_iterator &end, float lineSpacingMod, map_size maxHeight);
    // Gets the height and width of the line range
    map_size getBitmapHeight(const line_iterator &start, const line_iterator &end, float lineSpacingMod);
    map_size getBitmapWidth(const line_iterator &start, const line_iterator &end);
    // Compute all values for line rendering
    void getLinesRenderData(const line_iterator &start, const line_iterator &end, int channels, float lineSpacingMod, map_size &lineHeight, map_size &width, map_size &height, int &offset, int &fall);
};

#endif
