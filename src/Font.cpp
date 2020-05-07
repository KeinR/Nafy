#include "Font.h"

#include <iostream>
#include <fstream>
#include <cmath>

#include "env.h"
#include "defs.h"

nafy::Font::Font(const char *filepath) {
    std::ifstream file(getPath(PATH_FONTS + std::string(filepath)));
    if (!file.good()) {
        std::cerr << "ERROR: Failed to open font file at \"" << filepath << std::endl;
        buffer = nullptr;
        return;
    }

    file.seekg(0, file.end);
    const int bufferSize = file.tellg();
    file.seekg(0, file.beg);

    buffer = new unsigned char[bufferSize];

    file.read((char *)buffer, bufferSize);
    if (!file.good()) {
        std::cerr << "ERROR: Failed to read from font file at \"" << filepath << "\"" << std::endl;
        return;
    }

    file.close();
    if (!file.good()) {
        std::cerr << "WARNING: Failed to close font file at \"" << filepath << "\"" << std::endl;
    }

    if (!stbtt_InitFont(&info, buffer, 0)) {
        std::cerr << "ERROR: Failed to parse font file at \"" << filepath << "\"" << std::endl;
        return;
    }
    spaceGlyph = stbtt_FindGlyphIndex(&info, ' ');
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
}

nafy::Font::~Font() {
    if (buffer != nullptr) {
        delete[] buffer;
    }
}

std::vector<nafy::wrapLine> nafy::Font::wrapString(const fontMetrics *metrics, const std::vector<int> &str, const int wrapWidth) {

    std::vector<wrapLine> lines;
    int fx = 0; // Stands for like "font-x" or something
    bool canHazSpace = false;
    std::vector<int> sizeHistory;
    sizeHistory.reserve(str.size());
    for (std::vector<int>::size_type i = 0; i < str.size(); i++) {
        int aw;
        stbtt_GetGlyphHMetrics(&info, str[i], &aw, nullptr);

        int inc = std::round(aw * metrics->scale);

        if (i+1 < str.size()) {
            inc += std::round(stbtt_GetGlyphKernAdvance(&info, str[i], str[i+1]) * metrics->scale);
        }

        sizeHistory[i] = inc;

        if (fx + inc >= wrapWidth) {
            // Remove last char that caused overflow
            std::vector<int>::size_type resumeIncl = i;
            // Mov to last word for soft wrap- if there's more than one word...
            int width = fx;
            std::vector<int>::size_type terminateIndexExcl = i;
            fx = inc;
            if (canHazSpace) {
                canHazSpace = false;
                for (std::vector<int>::size_type newI = terminateIndexExcl; newI < str.size(); newI--) {
                    if (str[newI] == spaceGlyph) {
                        resumeIncl = newI+1;
                        for (; newI >= 0 && str[newI] == spaceGlyph; newI--);
                        newI++; // because it's exclusive
                        for (;terminateIndexExcl > newI;) {
                            terminateIndexExcl--;
                            width -= sizeHistory[terminateIndexExcl];
                            fx += sizeHistory[terminateIndexExcl];
                        }
                        terminateIndexExcl = newI;
                        break;
                    }
                }
            }
            lines.push_back({
                terminateIndexExcl,
                resumeIncl,
                width
            });
        } else {
            fx += inc;
            if (str[i] == spaceGlyph) {
                canHazSpace = true;
            }
        }
    }
    // Conditionally unnecessary, however for time being we'll keep it
    lines.push_back({
        str.size(),
        str.size(),
        fx
    });
    return lines;
}

std::vector<int> nafy::Font::indexString(const std::string &str) {
    std::vector<int> index;
    index.reserve(str.length());
    for (std::string::const_iterator it = str.cbegin(), end = str.cend(); it != end; ++it) {
        std::cout << "[[[" << *it << "->" << stbtt_FindGlyphIndex(&info, *it) << std::endl;
        index.push_back(stbtt_FindGlyphIndex(&info, *it));
    }
    return index;
}

int nafy::Font::glyphStrPixelWidth(const fontMetrics *metrics, const std::vector<int> &glyphs) {
    if (!glyphs.size()) {
        return 0;
    }
    int width = 0;
    for (std::vector<int>::const_iterator it = glyphs.cbegin(), end = glyphs.cend();;) {
        int aw;
        stbtt_GetGlyphHMetrics(&info, *it, &aw, nullptr);

        width += std::round(aw * metrics->scale);
        if (it+1 != end) {
            width += std::round(stbtt_GetGlyphKernAdvance(&info, *it, *(++it)) * metrics->scale);
        } else {
            break;
        }
    }
    return width;
}

void nafy::Font::render(const fontMetrics *metrics,
                std::vector<int>::const_iterator it, std::vector<int>::const_iterator endExcl,
                unsigned char *out, int outWidth) {

    if (it == endExcl) {
        return;
    }

    int fx = 0;
    int debug_count = 0;
    for (;;) {
        debug_count++;
        int aw;
        int lsb;
        stbtt_GetGlyphHMetrics(&info, *it, &aw, &lsb);

        // Get bounding box
        // top-left & bottom right respectively
        int x1, y1, x2, y2;
        stbtt_GetGlyphBitmapBox(&info, *it, metrics->scale, metrics->scale, &x1, &y1, &x2, &y2);

        // Get y
        int y = metrics->ascent + y1;

        // render font
        int byteOffset = fx + std::round(lsb * metrics->scale) + (y * outWidth);
        // std::cout << "making" << std::endl;
        int wid = x2 - x1;
        stbtt_MakeGlyphBitmap(&info, out + byteOffset, wid, y2 - y1, outWidth, metrics->scale, metrics->scale, *it);
        // if (it+1 >= endExcl) {
        //     std::cout << "BREACH," << std::endl;
        //     std::cout << "byteOffset=" << byteOffset << std::endl;
        //     std::cout << "*it=" << *it << std::endl;
        //     // delete[] out;
        //     // std::cout << out[100] << std::endl;
        //     std::cout << "CLEAR!" << std::endl;
        // }
        std::cout << *it << ": aw=" << std::round(aw * metrics->scale) << ", facto=" << wid << ", lsb=" << lsb;

        // fx += aw > wid ? aw : wid;
        fx += std::round(aw * metrics->scale);

        if (it+1 < endExcl) {
            std::cout << ", kern=" << (stbtt_GetGlyphKernAdvance(&info, *it, *(it+1))) << std::endl;
            fx += std::round(stbtt_GetGlyphKernAdvance(&info, *it, *(++it)) * metrics->scale);
        } else {
            std::cout << std::endl;
            break;
        }
    }
}

nafy::fontMetrics nafy::Font::getMetrics(int fontSize) {
    fontMetrics met;
    met.scale = stbtt_ScaleForPixelHeight(&info, fontSize);
    met.ascent = std::round(ascent * met.scale);
    met.descent = std::round(descent * met.scale);
    met.lineGap = std::round(lineGap * met.scale);
    return met;
}
