#ifndef TEXTCRAWL_H_INCLUDED
#define TEXTCRAWL_H_INCLUDED

#include <vector>

#include "Text.h"

/*
* Line it's super Text, except it progressively renders each glyph
* through calls to advance()
*/

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

    // Update the image
    inline void updateTex();
    // Update the metadata for the current line
    inline void updateLineData();
    // Prepare to render the lines
    void loadLines(const Font::line_iterator &start, const Font::line_iterator &end) override;

    void crawlSteal(TextCrawl &other);
    void crawlCopy(const TextCrawl &other);
    inline void crawlCopyIL(const TextCrawl &other);
    inline void crawlCopyPOD(const TextCrawl &other);
    // Delete the wrapped bitmap
    inline void delBitmap();

    // Render the next glyph
    bool doAdvance();
public:
    // Takes default font and shader
    TextCrawl();
    // Take given font and shader
    TextCrawl(const Font::type &font, const nafy::shader_t &shader);
    TextCrawl(const TextCrawl &other);
    TextCrawl(TextCrawl &&other);
    TextCrawl &operator=(const TextCrawl &other);
    TextCrawl &operator=(TextCrawl &&other);
    // Moving a Text into a TextCrawl would be a weird thing to do (one)
    // as well as breaking a few things (two)
    Text &operator=(const Text &other) = delete;
    Text &operator=(Text &&other) = delete;
    ~TextCrawl();
    // Does what Text does, however also deleting the bitmap
    void clear() override;
    // Calls reset() and loads the set string
    // void generate();
    // Resets to the start
    void reset();
    // Renders the next glyph.
    // Invokes undefined behavior if `generate()` hasn't been called yet.
    // returns `true` if reached the end
    bool advance();
    // Effectively calls `advance()` `count` times, same return
    bool advance(int count);
};

#endif

