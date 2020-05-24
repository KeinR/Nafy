#include "Face.h"

#include <iostream>
#include <cmath>

// A specific round function, rounds a 26.6 int to nearest 32
inline static int rto32(int v26_6) {
    return (v26_6 >> 6) + ((v26_6 & 0x3F) < 32 ? 0 : 1);
}

// For lsb offset calculations: negate to get positive, shift 6 times to obtain 26.6
inline static int compXOfs(int lsb) {
    return -lsb << 6;
}

// Struct constructors

Face::renderConf::renderConf() {
}
Face::renderConf::renderConf(const int channels, float *cMod, unsigned char *bitmap, unsigned int verticalStride, map_size lineHeight):
    channels(channels), cMod(cMod), bitmap(bitmap), verticalStride(verticalStride), lineHeight(lineHeight) {
}

Face::vec2::vec2() {
}
Face::vec2::vec2(map_size x, map_size y): x(x), y(y) {
}

// End struct constructors

Face::Face(FT_Face face): face(face),
    space(FT_Get_Char_Index(face, ' ')),
    newline(FT_Get_Char_Index(face, '\n')) {

    setSize(defaultSize);
}

Face::Face() {
}

Face::~Face() {
    if (face != nullptr) {
        FT_Done_Face(face);
    }
}

void Face::steal(Face &other) {
    FT_Done_Face(face);
    face = other.face;
    other.face = nullptr;
}

Face::Face(Face &&other) {
    steal(other);
}

Face &Face::operator=(Face &&other) {
    steal(other);
    return *this;
}

FT_Face Face::getFace() {
    return face;
}

FT_Error Face::setSize(unsigned int size) {
    return FT_Set_Pixel_Sizes(face, 0, size);
}

Face::line Face::consLine(const glyph_iterator &start, const glyph_iterator &end, const ofs_type horizontalOffset, const map_size width) {
    return line{start, end, horizontalOffset, width};
}

void Face::stringMetrics(glyph_iterator start, const glyph_iterator &end, map_size *width, map_size *height, ofs_type *horizontalOffset, ofs_type *verticalOffset) {
    if (start >= end) {
        return;
    }
    map_size preWidth = 0;
    map_size preHeight = 0;
    ofs_type preHorizontalOffset = 0;
    ofs_type preVerticalOffset = 0;
    map_size sHeight = 0;
    for (;; ++start) {
        FT_Error error = FT_Load_Glyph(face, *start, FT_LOAD_NO_BITMAP);
        if (!error) { // TODO: Pre-index strings with glyph indices
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
                FT_Get_Kerning(face, *start, *(start + 1), FT_KERNING_UNFITTED, &kerning);
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

Face::map_size Face::stringDescent(glyph_iterator start, const glyph_iterator &end) {
    map_size max = 0;
    for (; start < end; ++start) {
        FT_Error error = FT_Load_Glyph(face, *start, FT_LOAD_NO_BITMAP);
        if (!error) {
            const map_size lower = face->glyph->metrics.height - face->glyph->metrics.horiBearingY;
            if (lower > max) {
                max = lower;
            }
        } else {
            std::cerr << "Failed to load char: 0x" << std::hex << error << std::dec << std::endl;
        }
    }
    return max;
}

Face::map_size Face::stringAscent(glyph_iterator start, const glyph_iterator &end) {
    map_size max = 0;
    for (; start < end; ++start) {
        FT_Error error = FT_Load_Glyph(face, *start, FT_LOAD_NO_BITMAP);
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

std::vector<Face::char_metrics> Face::measureString(glyph_iterator start, const glyph_iterator &end) {
    std::vector<char_metrics> result;
    if (start >= end) {
        return result;
    }
    result.reserve(end - start);
    for (;; ++start) {
        FT_Error error = FT_Load_Glyph(face, *start, FT_LOAD_NO_BITMAP);
        if (!error) {
            result.push_back(char_metrics{face->glyph->bitmap_left, 0, face->glyph->advance.x});
            if (start + 1 < end) {
                FT_Vector kerning;
                FT_Get_Kerning(face, *start, *(start + 1), FT_KERNING_UNFITTED, &kerning);
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

unsigned char *Face::renderBitmap(const glyph_iterator &start, const glyph_iterator &end, const int channels, float cMod[], map_size &width, map_size &height) {
    int horizontalOffset;
    stringMetrics(start, end, &width, &height, &horizontalOffset, nullptr);
    unsigned char *bitmap = new unsigned char[width * height * channels]{0};
    renderTo(start, end, renderConf(channels, cMod, bitmap, width, height), horizontalOffset);
    return bitmap;
}

void Face::renderTo(glyph_iterator start, const glyph_iterator &end, const renderConf &render, ofs_type horizontalOffset) {
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

void Face::renderGlyphTo(glyph_type glyph, glyph_type next, const renderConf &render, vec2 &pen) {
    if (!FT_Load_Glyph(face, glyph, FT_LOAD_RENDER)) {
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

unsigned char *Face::renderLines(line_iterator start, const line_iterator &end, const int channels, float cMod[], float lineSpacingMod, map_size &width, map_size &height) {
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

Face::line Face::makeLine(const glyph_iterator &start, const glyph_iterator &end) {
    line result;
    result.start = start;
    result.end = end;
    stringMetrics(start, end, &result.width, nullptr, &result.horizontalOffset, nullptr);
    result.width <<= 6;
    return result;
}

Face::line_str Face::getLines(const glyph_iterator &start, const glyph_iterator &end, unsigned int wrappingWidth) {
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
                        for (glyph_iterator newI = stop; newI >= start; newI--) {
                            if (*newI == space) {
                                resume = newI + 1;
                                for (; newI >= start && *newI == space; newI--);
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
                lines.push_back(consLine(begin, stop, compXOfs(metrics[begin - start].lsb), thisWidth)); // negate to get positive, shift 6 times to obtain 26.6, and shift 7th time to muntiply by 2
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
    lines.push_back(consLine(begin, end, compXOfs(metrics[begin - start].lsb), width));

    return lines;
}

Face::glyph_str Face::indexString(std::string::const_iterator start, const std::string::const_iterator &end) {
    glyph_str index;
    index.reserve(end - start);
    for (; start < end; start++) {
        index.push_back(FT_Get_Char_Index(face, *start));
    }
    return index;
}

std::vector<Face::line_iterator> Face::breakupLines(line_iterator start, const line_iterator &end, float lineSpacingMod, map_size maxHeight) {
    std::vector<Face::line_iterator> result;

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

Face::line_iterator Face::truncLines(line_iterator start, const line_iterator &end, float lineSpacingMod, map_size maxHeight) {

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

Face::map_size Face::getBitmapHeight(const line_iterator &start, const line_iterator &end, float lineSpacingMod) {
    const int deduction = rto32(std::round(face->size->metrics.height * (lineSpacingMod - 1)));
    const int descent = rto32(std::round(face->size->metrics.height * lineSpacingMod));
    return (end - start) * descent - deduction;
}

Face::map_size Face::getBitmapWidth(const line_iterator &start, const line_iterator &end) {
    map_size width = 0;
    for (line_iterator it = start; it < end; it++) {
        if (it->width > width) {
            width = it->width;
        }
    }
    return rto32(width); // Because the width stored in the lines is 26.6
}

void Face::getLinesRenderData(const line_iterator &start, const line_iterator &end, int channels, float lineSpacingMod, map_size &lineHeight, map_size &width, map_size &height, int &offset, int &fall) {
    if (start >= end) {
        return;
    }
    const Face::map_size cap = stringAscent(start->start, start->end);
    // https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_size_metrics
    // "The height in 26.6 fractional pixels, rounded to an integer value"
    // Rounded to the nearest pixel coordinate or 26.6 fractional value?
    // Eh, best to be on the safe side. 
    Face::map_size descent = std::round(face->size->metrics.height * lineSpacingMod); // 26.6

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
