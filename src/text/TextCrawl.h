#ifndef TEXTCRAWL_H_INCLUDED
#define TEXTCRAWL_H_INCLUDED

#include <vector>

#include "Text.h"

class TextCrawl: public Text {
    Font::line_iterator currentLine;
    Font::line_iterator lastLineX;

    // Per-line
    Font::glyph_iterator linePos;
    Font::vec2 pen;

    unsigned char *bitmap;
    std::size_t bmpSizeBytes;
    int fall;
    unsigned char *start; // Start offset
    Font::renderConf render_c;

    inline void updateTex();
    inline void updateLineData();
    void loadLines(const Font::line_iterator &start, const Font::line_iterator &end) override;

    void crawlSteal(TextCrawl &other);
    void crawlCopy(const TextCrawl &other);
    inline void crawlCopyIL(const TextCrawl &other);
    inline void crawlCopyPOD(const TextCrawl &other);
    inline void delBitmap();

    bool doAdvance();
public:
    // If made with this, must be set equal to another or have the face and shader set
    // before calling ANY other methods, otherwise undefined behavior is invoked
    TextCrawl();
    TextCrawl(const Font::type &font, const unsigned int shader);
    TextCrawl(const TextCrawl &other);
    TextCrawl(TextCrawl &&other);
    TextCrawl &operator=(const TextCrawl &other);
    TextCrawl &operator=(TextCrawl &&other);
    Text &operator=(const Text &other) = delete;
    Text &operator=(Text &&other) = delete;
    ~TextCrawl();
    // Calls reset() and loads the set string
    // void generate();
    // Resets to the start
    void reset();
    // Moves forward one char.
    // Invokes undefined behavior if `generate()` hasn't been called yet.
    // returns `true` if reached the end
    bool advance();
    // Effectively calls `advance()` `count` times, same return
    bool advance(int count);
};

#endif

