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

    inline void crawlSteal(TextCrawl &other);
    inline void crawlCopy(TextCrawl &other);
    inline void crawlCopyIL(TextCrawl &other);
    inline void crawlCopyPOD(TextCrawl &other);
    inline void delBitmap();
public:
    // If made with this, must be set equal to another or have the face and shader set
    // before calling ANY other methods, otherwise undefined behavior is invoked
    TextCrawl();
    TextCrawl(Face &face, const unsigned int shader);
    TextCrawl(const TextCrawl &other);
    TextCrawl(TextCrawl &&other);
    TextCrawl &operator=(const TextCrawl &other);
    TextCrawl &operator=(TextCrawl &&other);
    Text(const Text &other) = delete;
    Text(Text &&other) = delete;
    Text &operator=(const Text &other) = delete;
    Text &operator=(Text &&other) = delete;
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

