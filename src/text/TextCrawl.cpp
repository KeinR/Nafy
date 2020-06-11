#include "TextCrawl.h"

#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

#include "textdefs.h"

#include GLFW_INCLUDE_HEADER_LOCATION

#define CHANNELS 4

TextCrawl::TextCrawl():
    Text(),
    pen(0, 0), bitmap(nullptr),
    fall(0), start(nullptr),
    render_c(CHANNELS, color.get(), nullptr, 0, 0) {
}

TextCrawl::TextCrawl(const Font::type &font, const nafy::shader_t &shader):
    Text(font, shader),
    pen(0, 0), bitmap(nullptr),
    fall(0), start(nullptr),
    render_c(CHANNELS, color.get(), nullptr, 0, 0) {
}

// Start mem manegment

void TextCrawl::crawlSteal(TextCrawl &other) {

    crawlCopyIL(other);
    crawlCopyPOD(other);

    pen = std::move(other.pen);

    delBitmap();
    bitmap = other.bitmap;
    other.bitmap = nullptr;
    render_c = std::move(other.render_c);
}

void TextCrawl::crawlCopy(const TextCrawl &other) {

    crawlCopyIL(other);
    crawlCopyPOD(other);

    pen = other.pen;

    delBitmap();
    const std::size_t length = bmpSizeBytes / sizeof(unsigned char);
    bitmap = new unsigned char[length];
    std::copy(other.bitmap, other.bitmap + length, bitmap);
    render_c = other.render_c;
}

void TextCrawl::crawlCopyIL(const TextCrawl &other) {
    currentLine = lines.begin() + (currentLine - other.lines.begin());
    lastLineX = lines.begin() + (lastLineX - other.lines.begin());
    linePos = currentLine->start + (linePos - other.currentLine->start);
}

void TextCrawl::crawlCopyPOD(const TextCrawl &other) {
    bmpSizeBytes = other.bmpSizeBytes;
    fall = other.fall;
    start = other.start;
}

void TextCrawl::delBitmap() {
    if (bitmap != nullptr) {
        delete[] bitmap;
        bitmap = nullptr;
    }
}

TextCrawl::TextCrawl(const TextCrawl &other) {
    textCopy(other);
    crawlCopy(other);
}

TextCrawl::TextCrawl(TextCrawl &&other): Text(std::move(other)) {
    crawlSteal(other);
}

TextCrawl &TextCrawl::operator=(const TextCrawl &other) {
    textCopy(other);
    crawlCopy(other);
    return *this;
}

TextCrawl &TextCrawl::operator=(TextCrawl &&other) {
    textSteal(other);
    crawlSteal(other);
    return *this;
}

TextCrawl::~TextCrawl() {
    delBitmap();
}

// end mem manegment

void TextCrawl::updateTex() {
    image.setImage(GL_RGBA, image.getWidth(), image.getHeight(), bitmap);
}

void TextCrawl::updateLineData() {
    linePos = currentLine->start;
    pen.x = currentLine->horizontalOffset;
    pen.y = 0;
}

void TextCrawl::loadLines(const Font::line_iterator &begin, const Font::line_iterator &end) {
    int offset;
    configureFont();
    unsigned int width, height;
    font->getLinesRenderData(begin, end, CHANNELS, lineSpacingMod, render_c.lineHeight, width, height, offset, fall);
    image.setWidth(width);
    image.setHeight(height);
    render_c.verticalStride = width;

    bmpSizeBytes = width * height * CHANNELS;

    delBitmap();
    bitmap = new unsigned char[bmpSizeBytes];

    // I get a literal 50% performance for these lines
    // and last from using memset as opposed to {0}
    // initializing the bitmap.
    // Literally, with a 110x44 bitmap, bitmap = new unsigned char[bmpSizeBytes]{0};
    // takes like 100+ microseconds, whereas bitmap = new unsigned char[bmpSizeBytes];
    // plus the below lines takes as high as 50 and as low as 30 microseconds
    bmpSizeBytes *= sizeof(unsigned char);
    std::memset(bitmap, 0, bmpSizeBytes); 

    start = bitmap + offset;
    render_c.bitmap = start;

    currentLine = begin;
    lastLineX = end;

    updateLineData();
    updateTex();
}

void TextCrawl::reset() {
    std::memset(bitmap, 0, bmpSizeBytes);
    render_c.bitmap = start;
    lastLineX = stops[stopsIndex];
    if (stopsIndex != 0) {
        currentLine = stops[stopsIndex - 1];
    } else {
        currentLine = lines.cbegin();
    }
    updateLineData();
}

bool TextCrawl::doAdvance() {
    if (!lines.size()) { // Not going to account for all situations - be careful about when you call advance!
        return true;
    }
    if (linePos >= currentLine->end) {
        if (currentLine + 1 >= lastLineX) {
            return true;
        }
        ++currentLine;
        render_c.bitmap -= fall;
        updateLineData();
    }

    font->renderGlyphTo(*linePos, linePos + 1 < currentLine->end ? *(linePos + 1) : 0, render_c, pen);

    ++linePos;
    return false;
}

void TextCrawl::clear() {
    image.setImage(GL_RGBA, 0, 0, nullptr);
    image.setWidth(0);
    image.setHeight(0);
    lines.clear();
    delBitmap();
    bmpSizeBytes = 0;
}

bool TextCrawl::advance() {
    configureFont();
    const bool ret = doAdvance();
    updateTex();
    return ret;
}

bool TextCrawl::advance(int count) {
    configureFont();
    bool ret = false;
    // The legendary arrow operator
    while (count --> 0) {
        if (doAdvance()) {
            ret = true;
            break;
        }
    }
    updateTex();
    return ret;
}
