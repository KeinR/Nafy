#include "TextCrawl.h"

#include <iostream>
#include <cstring>
#include <cmath>

#include "textdefs.h"

#include GLFW_INCLUDE_HEADER_LOCATION

#define CHANNELS 4

TextCrawl::TextCrawl(Face &face, const unsigned int shader):
    Text(face, shader),
    pen(0, 0), bitmap(nullptr),
    fall(0), start(nullptr),
    render_c(CHANNELS, color, nullptr, 0, 0) {
}

TextCrawl::~TextCrawl() {
    if (bitmap != nullptr) {
        delete[] bitmap;
    }
}

void TextCrawl::updateTex() {
    glBindTexture(GL_TEXTURE_2D, TX);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderedWidth, renderedHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
}

void TextCrawl::updateLineData() {
    linePos = currentLine->start;
    pen.x = currentLine->horizontalOffset;
    pen.y = 0;
}

void TextCrawl::loadLines(const Face::line_iterator &begin, const Face::line_iterator &end) {
    int offset;
    face->getLinesRenderData(begin, end, CHANNELS, lineSpacingMod, render_c.lineHeight, renderedWidth, renderedHeight, offset, fall);
    render_c.verticalStride = renderedWidth;

    bmpSizeBytes = renderedWidth * renderedHeight * CHANNELS;

    if (bitmap != nullptr) {
        delete[] bitmap;
    }
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

bool TextCrawl::advance() {
    if (linePos >= currentLine->end) {
        if (currentLine + 1 >= lastLineX) {
            return true;
        }
        ++currentLine;
        render_c.bitmap -= fall;
        updateLineData();
    }

    face->renderGlyphTo(*linePos, linePos + 1 < currentLine->end ? *(linePos + 1) : 0, render_c, pen);

    ++linePos;
    updateTex();
    return false;
}
