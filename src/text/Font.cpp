#include "Font.h"

#include <iostream>
#include <cmath>

constexpr unsigned int newline = -1;

// A specific round function, rounds a 26.6 int to nearest 32
inline static int rto32(int v26_6) {
    return (v26_6 >> 6) + ((v26_6 & 0x3F) < 32 ? 0 : 1);
}

// For lsb offset calculations: negate to get positive, shift 6 times to obtain 26.6
inline static int compXOfs(int lsb) {
    return -lsb << 6;
}

inline static int compOffset(int lsb, Font::textAlign align, unsigned int &width, unsigned int wrappingWidth) {
    Font::ofs_type xOffset = compXOfs(lsb);
    // TODO: The performance impact is negnigable, but is there a better way,
    // like using function pointers?
    switch (align) {
        case Font::textAlign::left:
            break;
        case Font::textAlign::center: {
            // TODO: Perhaps ceil?
            Font::ofs_type shift = (wrappingWidth - width) / 2;
            xOffset += shift;
            width += shift;
            break;
        }
        case Font::textAlign::right:
            xOffset += wrappingWidth - width;
            width = wrappingWidth;
            break;
    }
    return xOffset;
}

// Struct constructors

Font::renderConf::renderConf() {
}
Font::renderConf::renderConf(const int channels, float *cMod, unsigned char *bitmap, unsigned int verticalStride, map_size lineHeight):
    channels(channels), cMod(cMod), bitmap(bitmap), verticalStride(verticalStride), lineHeight(lineHeight) {
}

Font::vec2::vec2() {
}
Font::vec2::vec2(map_size x, map_size y): x(x), y(y) {
}

// End struct constructors

Font::Font(FT_Face face, const std::shared_ptr<unsigned char> &data):
    face(face), data(data),
    space(FT_Get_Char_Index(face, ' ')),
    fontNewline(FT_Get_Char_Index(face, '\n')) {

    setSize(defaultSize);
}

Font::Font(): face(nullptr) {
}

Font::~Font() {
    delFace();
}

Font::glyph_type Font::getGlyph(glyph_type glyph) {
    if (glyph == newline) {
        return fontNewline;
    }
    return glyph;
}

void Font::delFace() {
    if (face != nullptr) {
        FT_Done_Face(face);
    }
}

void Font::steal(Font &other) {
    delFace();
    face = other.face;
    other.face = nullptr;
    copyPOD(other);
}

void Font::copyPOD(const Font &other) {
    space = other.space;
    fontNewline = other.fontNewline;
}

Font::Font(Font &&other) {
    steal(other);
}

Font &Font::operator=(Font &&other) {
    steal(other);
    return *this;
}

FT_Face Font::getFace() {
    return face;
}

FT_Error Font::setSize(unsigned int size) {
    return FT_Set_Pixel_Sizes(face, 0, size);
}

Font::line Font::consLine(const glyph_iterator &start, const glyph_iterator &end, const ofs_type horizontalOffset, const map_size width) {
    return line{start, end, horizontalOffset, width};
}

void Font::stringMetrics(glyph_iterator start, const glyph_iterator &end, map_size *width, map_size *height, ofs_type *horizontalOffset, ofs_type *verticalOffset) {
    if (start >= end) {
        return;
    }
    map_size preWidth = 0;
    map_size preHeight = 0;
    ofs_type preHorizontalOffset = 0;
    ofs_type preVerticalOffset = 0;
    map_size sHeight = 0;
    for (;; ++start) {
        const glyph_type g = getGlyph(*start);
        FT_Error error = FT_Load_Glyph(face, g, FT_LOAD_NO_BITMAP);
        if (!error) {
            preWidth += face->glyph->advance.x;
            preHeight += face->glyph->advance.y;
            if (face->glyph->metrics.horiBearingY > preVerticalOffset) {
                preVerticalOffset = face->glyph->metrics.horiBearingY;
            }
            if (face->glyph->metrics.height > sHeight) {
                sHeight = face->glyph->metrics.height;
            }
            if (face->glyph->bitmap_left < preHorizontalOffset) {
                preHorizontalOffset = face->glyph->bitmap_left;
            }
            if (start + 1 < end) {
                FT_Vector kerning;
                FT_Get_Kerning(face, g, getGlyph(*(start + 1)), FT_KERNING_UNFITTED, &kerning);
                preWidth += kerning.x;
                preHeight += kerning.y;
            } else {
                break;
            }
        } else {
            std::cerr << "Failed to load char: 0x" << std::hex << error << std::dec << std::endl;
        }
    }
    if (width != nullptr) {
        *width = rto32(preWidth) - preHorizontalOffset;
    }
    if (height != nullptr) {
        *height = rto32(preHeight + sHeight);
    }
    if (horizontalOffset != nullptr) {
        *horizontalOffset = compXOfs(preHorizontalOffset);
    }
    if (verticalOffset != nullptr) {
        *verticalOffset = preVerticalOffset;
    }
}

int Font::stringDescent(glyph_iterator start, const glyph_iterator &end) {
    int max = -0x7FFFFFFF;
    for (; start < end; ++start) {
        FT_Error error = FT_Load_Glyph(face, getGlyph(*start), FT_LOAD_NO_BITMAP);
        if (!error) {
            const int lower = face->glyph->metrics.height - face->glyph->metrics.horiBearingY;
            if (lower > max) {
                max = lower;
            }
        } else {
            std::cerr << "Failed to load char: 0x" << std::hex << error << std::dec << std::endl;
        }
    }
    return max;
}

Font::map_size Font::stringAscent(glyph_iterator start, const glyph_iterator &end) {
    map_size max = 0;
    for (; start < end; ++start) {
        FT_Error error = FT_Load_Glyph(face, getGlyph(*start), FT_LOAD_NO_BITMAP);
        if (!error) {
            if (face->glyph->metrics.horiBearingY > max) {
                max = face->glyph->metrics.horiBearingY;
            }
        } else {
            std::cerr << "Failed to load char: 0x" << std::hex << error << std::dec << std::endl;
        }
    }
    return max;
}

std::vector<Font::char_metrics> Font::measureString(glyph_iterator start, const glyph_iterator &end) {
    std::vector<char_metrics> result;
    if (start >= end) {
        return result;
    }
    result.reserve(end - start);
    for (;; ++start) {
        const glyph_type g = getGlyph(*start);
        FT_Error error = FT_Load_Glyph(face, g, FT_LOAD_NO_BITMAP);
        if (!error) {
            result.push_back(char_metrics{face->glyph->bitmap_left, 0, face->glyph->advance.x});
            if (start + 1 < end) {
                FT_Vector kerning;
                FT_Get_Kerning(face, g, getGlyph(*(start + 1)), FT_KERNING_UNFITTED, &kerning);
                result.back().kern = kerning.x;
            } else {
                break;
            }
        } else {
            std::cerr << "Failed to load char: 0x" << std::hex << error << std::dec << std::endl;
        }
    }
    return result;
}

unsigned char *Font::renderBitmap(const glyph_iterator &start, const glyph_iterator &end, const int channels, float cMod[], map_size &width, map_size &height) {
    int horizontalOffset;
    stringMetrics(start, end, &width, &height, &horizontalOffset, nullptr);
    unsigned char *bitmap = new unsigned char[width * height * channels]{0};
    renderTo(start, end, renderConf(channels, cMod, bitmap, width, height), horizontalOffset);
    return bitmap;
}

void Font::renderTo(glyph_iterator start, const glyph_iterator &end, const renderConf &render, ofs_type horizontalOffset) {
    if (start >= end) {
        return;
    }
    vec2 pen(horizontalOffset, 0);
    for (;; ++start) {
        if (start + 1 < end) {
            renderGlyphTo(*start, *(start + 1), render, pen);
        } else {
            renderGlyphTo(*start, 0, render, pen);
            break;
        }
    }
}

void Font::renderGlyphTo(glyph_type glyph, glyph_type next, const renderConf &render, vec2 &pen) {
    if (!FT_Load_Glyph(face, getGlyph(glyph), FT_LOAD_RENDER)) {
        const int ix = rto32(pen.x) + face->glyph->bitmap_left;
        const int iy = rto32(pen.y) - face->glyph->bitmap_top;
        for (unsigned int y = 0; y < face->glyph->bitmap.rows; y++) {
            for (unsigned int x = 0; x < face->glyph->bitmap.width; x++) {
                const int srcIndex = x + y * face->glyph->bitmap.width;
                if (face->glyph->bitmap.buffer[srcIndex]) {
                    const int insIndex = ((ix + x) + (render.lineHeight - 1 - (iy + y)) * render.verticalStride) * render.channels;
                    for (int c = 0; c < render.channels; c++) {
                        render.bitmap[insIndex + c] = std::round(face->glyph->bitmap.buffer[srcIndex] * render.cMod[c]);
                    }
                }
            }
        }

        pen.x += face->glyph->advance.x;
        pen.y += face->glyph->advance.y;
        if (next) {
            FT_Vector kerning;
            FT_Get_Kerning(face, glyph, next, FT_KERNING_UNFITTED, &kerning);
            pen.x += kerning.x;
            pen.y += kerning.y;
        }
    } else {
        // TODO: Proper error handling
        std::cerr << "FAILED TO LOAD GLYOH" << std::endl;
    }
}

unsigned char *Font::renderLines(line_iterator start, const line_iterator &end, const int channels, float cMod[], float lineSpacingMod, map_size &width, map_size &height) {
    map_size lineHeight;
    int offset;
    int fall;
    getLinesRenderData(start, end, channels, lineSpacingMod, lineHeight, width, height, offset, fall);

    unsigned char *bitmap = new unsigned char[width * height * channels]{0};

    renderConf render(channels, cMod, nullptr, width, lineHeight);
    for (; start < end; ++start) {

        render.bitmap = bitmap + offset;
        renderTo(start->start, start->end, render, start->horizontalOffset);

        offset -= fall;
    }
    return bitmap;
}

Font::line Font::makeLine(const glyph_iterator &start, const glyph_iterator &end) {
    line result;
    result.start = start;
    result.end = end;
    stringMetrics(start, end, &result.width, nullptr, &result.horizontalOffset, nullptr);
    result.width <<= 6;
    return result;
}

Font::line_str Font::getLines(const glyph_iterator &start, const glyph_iterator &end, unsigned int wrappingWidth, const textAlign align) {
    line_str lines;
    if (start >= end) {
        return lines;
    }
    wrappingWidth <<= 6; // Convert to 26.6 format for compatability; NOTE this could cause problems with values over 2^26-1
    std::vector<char_metrics> metrics = measureString(start, end);
    unsigned int width = compXOfs(metrics[0].lsb);
    bool spaceLast = false;
    glyph_iterator begin = start;
    for (glyph_iterator it = start; it < end; ++it) {
        if (*it == newline) {
            glyph_iterator stop = it;
            glyph_iterator resume = it;
            while (--stop >= start && *stop == space);
            stop++;
            while (++resume < end && *resume == space);
            lines.push_back(consLine(begin, stop, compXOfs(metrics[begin - start].lsb), width));
            begin = resume;
            width = 0;
        } else {
            int i = it - start;
            const int move = metrics[i].advance + metrics[i].kern;
            if (width + move > wrappingWidth) {
                glyph_iterator stop = it;
                glyph_iterator resume = it;
                unsigned int thisWidth = width;
                if (*it == space) {
                    // Check if the space can become analagous for a newline char:
                    // it has no adjacent spaces.
                    if (it - 1 >= start && *(it - 1) != space && it + 1 < end) {
                        const unsigned int c = *(it + 1);
                        if (c == newline) {
                            ++it;
                            resume = it;
                            width = 0;
                        } else if (c != space) {
                            ++resume;
                            width = 0;
                        } else {
                            width = move;
                        }
                    } else {
                        width = move;
                    }
                } else {
                    width = move;
                    if (spaceLast && *it != space) {
                        spaceLast = false;
                        for (glyph_iterator newI = stop; newI >= start; --newI) {
                            if (*newI == space) {
                                resume = newI + 1;
                                for (; newI >= start && *newI == space; --newI);
                                ++newI; // because it's exclusive
                                for (int fi; stop > newI;) {
                                    --stop;
                                    fi = stop - start;
                                    thisWidth -= metrics[fi].advance + metrics[fi].kern;
                                    width += metrics[fi].advance + metrics[fi].kern;
                                }
                                stop = newI;
                                break;
                            }
                        }
                    }
                }

                ofs_type xOffset = compOffset(metrics[begin - start].lsb, align, thisWidth, wrappingWidth);
                lines.push_back(consLine(begin, stop, xOffset, thisWidth)); // negate to get positive, shift 6 times to obtain 26.6, and shift 7th time to muntiply by 2
                begin = resume;
                width += compXOfs(metrics[resume - start].lsb);
            } else {
                width += move;
                if (*it == space) {
                    spaceLast = true;
                }
            }
        }
    }
    ofs_type xOffset = compOffset(metrics[begin - start].lsb, align, width, wrappingWidth);
    lines.push_back(consLine(begin, end, xOffset, width));

    return lines;
}

Font::glyph_str Font::indexString(std::string::const_iterator start, const std::string::const_iterator &end) {
    glyph_str index;
    index.reserve(end - start);
    for (; start < end; ++start) {
        if (*start == '\n') {
            index.push_back(newline);
        } else {
            index.push_back(FT_Get_Char_Index(face, *start));
        }
    }
    return index;
}

std::vector<Font::line_iterator> Font::breakupLines(line_iterator start, const line_iterator &end, float lineSpacingMod, map_size maxHeight) {
    std::vector<Font::line_iterator> result;

    const map_size deduction = rto32(std::round(face->size->metrics.height * (lineSpacingMod - 1)));
    const map_size descent = rto32(std::round(face->size->metrics.height * lineSpacingMod));

    maxHeight += deduction;

    while (start < end) {
        for (map_size height = 0;; height += descent) {
            if (height <= maxHeight) {
                start++;
                if (start >= end) {
                    result.push_back(end);
                    break;
                }
            } else {
                result.push_back(start);
                break;
            }
        }
    }

    return result;
}

Font::line_iterator Font::truncLines(line_iterator start, const line_iterator &end, float lineSpacingMod, map_size maxHeight) {

    const int deduction = rto32(std::round(face->size->metrics.height * (lineSpacingMod - 1)));
    const int descent = rto32(std::round(face->size->metrics.height * lineSpacingMod));

    maxHeight += deduction;

    for (unsigned int height = 0;; height += descent) {
        if (height <= maxHeight) {
            start++;
            if (start >= end) {
                return end;
            }
        } else {
            return start;
        }
    }
}

Font::map_size Font::getBitmapHeight(const line_iterator &start, const line_iterator &end, float lineSpacingMod) {
    const int deduction = rto32(std::round(face->size->metrics.height * (lineSpacingMod - 1)));
    const int descent = rto32(std::round(face->size->metrics.height * lineSpacingMod));
    return (end - start) * descent - deduction;
}

Font::map_size Font::getBitmapWidth(const line_iterator &start, const line_iterator &end) {
    map_size width = 0;
    for (line_iterator it = start; it < end; it++) {
        if (it->width > width) {
            width = it->width;
        }
    }
    return rto32(width); // Because the width stored in the lines is 26.6
}

void Font::getLinesRenderData(const line_iterator &start, const line_iterator &end, int channels, float lineSpacingMod, map_size &lineHeight, map_size &width, map_size &height, int &offset, int &fall) {
    if (start >= end) {
        return;
    }
    const Font::map_size cap = stringAscent(start->start, start->end);
    // https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_size_metrics
    // "The height in 26.6 fractional pixels, rounded to an integer value"
    // Rounded to the nearest pixel coordinate or 26.6 fractional value?
    // Eh, best to be on the safe side. 
    Font::map_size descent = std::round(face->size->metrics.height * lineSpacingMod); // 26.6

    // Hight per line
    lineHeight = rto32(descent);
    // bitmap width
    width = getBitmapWidth(start, end);
    // The deduction saves a little bit of memory; deduct the foot of empty space at the bottom of the line
    height = rto32((end - start - 1) * descent + cap + stringDescent((end-1)->start, (end-1)->end));
    // Start offset
    offset = (height - rto32(descent + cap)) * width * channels;
    // Offset deduction per line
    fall = width * channels * lineHeight;
}
