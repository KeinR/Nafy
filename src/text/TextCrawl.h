#ifndef TEXTCRAWL_H_INCLUDED
#define TEXTCRAWL_H_INCLUDED

#include <vector>

#include "Text.h"
#include "Face.h"

class TextCrawl: public Text {
    Face::line_iterator currentLine;
    Face::line_iterator lastLineX;

    // Per-line
    Face::glyph_iterator linePos;
    Face::vec2 pen;

    unsigned char *bitmap;
    std::size_t bmpSizeBytes;
    int fall;
    unsigned char *start; // Start offset
    Face::renderConf render_c;

    inline void updateTex();
    inline void updateLineData();
    void loadLines(const Face::line_iterator &start, const Face::line_iterator &end) override;
public:
    TextCrawl(Face &face, const unsigned int shader);
    ~TextCrawl();
    // Calls reset() and loads the set string
    // void generate();
    // Resets to the start
    void reset();
    // Moves forward one char
    // returns `true` if reached the end
    bool advance();
};

#endif

