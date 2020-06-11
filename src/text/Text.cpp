#include "Text.h"

#include <iostream>
#include <cmath>

#include "textdefs.h"

#include GLFW_INCLUDE_HEADER_LOCATION

Text::Text(): Text(getDefaultFont(), getDefaultTextShader()) {
}

Text::Text(const Font::type &font, const nafy::shader_t &shader):
    font(font),
    wrappingWidth(0), overflowHeight(0),
    lineSpacingMod(1.0f), textAlign(Font::textAlign::left),
    fontSize(Font::defaultSize), stopsIndex(0) {
    bindShader(shader);
}

// Memory manegemet

void Text::textSteal(Text &other) {
    font = std::move(other.font);
    image = std::move(other.image);
    color = std::move(other.color);

    str = std::move(other.str);
    stops = std::move(other.stops);

    textCopyIL(other);
    textCopyPOD(other);
}

void Text::textCopy(const Text &other) {
    font = other.font;
    image = other.image;
    color = other.color;

    str = other.str;
    stops = other.stops;

    textCopyIL(other);
    textCopyPOD(other);
}

void Text::textCopyIL(const Text &other) {
    // https://stackoverflow.com/questions/11021764/does-moving-a-vector-invalidate-iterators
    // Fucking committees
    index = other.index;
    lines = other.lines;
    for (Font::line &ln : lines) {
        ln.start = index.begin() + (ln.start - other.index.begin());
        ln.end = index.begin() + (ln.end - other.index.begin());
    }
    for (Font::line_iterator &it : stops) {
        it = lines.begin() + (it - other.lines.begin());
    }
}

void Text::textCopyPOD(const Text &other) {
    wrappingWidth = other.wrappingWidth;
    overflowHeight = other.overflowHeight;
    lineSpacingMod = other.lineSpacingMod;
    stopsIndex = other.stopsIndex;
    textAlign = other.textAlign;
}

Text::Text(const Text &other) {
    textCopy(other);
    if (lines.size()) {
        loadStops();
    }
}

Text::Text(Text &&other) {
    textSteal(other);
}

Text &Text::operator=(const Text &other) {
    textCopy(other);
    if (lines.size()) {
        loadStops();
    }
    return *this;
}

Text &Text::operator=(Text &&other) {
    textSteal(other);
    return *this;
}

// End memory manegemet

void Text::configureFont() {
    font->setSize(fontSize);
}

void Text::bindShader(const nafy::shader_t &shader) {
    image.bindShader(shader);
}

nafy::shader_t Text::getShader() {
    return image.getShader();
}

void Text::clear() {
    image.setImage(GL_RGBA, 0, 0, nullptr);
    image.setWidth(0);
    image.setHeight(0);
    lines.clear();
}

void Text::generate() {
    if (!str.size()) {
        clear();
        return;
    }
    for (std::string::size_type i = 0;; i++) {
        if (i >= str.size()) {
            clear();
            return;
        }
        if (str[i] != ' ' && str[i] != '\n') {
            break;
        }
    }

    configureFont();
    index = font->indexString(str.cbegin(), str.cend());
    if (wrappingWidth) {
        lines = font->getLines(index.cbegin(), index.cend(), wrappingWidth, textAlign);
        if (overflowHeight) {
            stops = font->breakupLines(lines.cbegin(), lines.cend(), lineSpacingMod, overflowHeight);
        } else {
            stops.clear();
            stops.push_back(lines.cend());
        }
    } else {
        lines.clear();
        lines.push_back(font->makeLine(index.cbegin(), index.cend()));
        stops.clear();
        stops.push_back(lines.cend());
    }

    loadLines(lines.cbegin(), stops[0]);
}

void Text::loadLines(const Font::line_iterator &start, const Font::line_iterator &end) {
    configureFont();
    unsigned int width, height;
    unsigned char *bitmap = font->renderLines(start, end, 4, color.get(), lineSpacingMod, width, height);
    image.setWidth(width);
    image.setHeight(height);
    image.setImage(GL_RGBA, width, height, bitmap);

    delete[] bitmap;
}

void Text::loadStops() {
    Font::line_iterator start;
    if (stopsIndex != 0) {
        start = stops[stopsIndex - 1];
    } else {
        start = lines.begin();
    }
    loadLines(start, stops[stopsIndex]);
}

void Text::render() {
    image.render();
}

bool Text::nextOverflow() {
    if (stopsIndex + 1 < overflowSize()) {
        stopsIndex++;
        loadStops();
        return true;
    }
    return false;
}

Font::line_str::size_type Text::overflowSize() {
    return stops.size();
}

void Text::seekOverflow(Font::line_str::size_type i) {
    if (i < overflowSize()) {
        stopsIndex = i;
        loadStops();
    }
}

/* Setter bloat */


nafy::Color &Text::getColor() {
    return color;
}

Font::type Text::getFont() {
    return font;
}

void Text::setFont(const Font::type &font) {
    this->font = font;
}

int Text::getX() {
    return image.getX();
}

int Text::getY() {
    return image.getY();
}

std::string &Text::getString() {
    return str;
}

Font::textAlign Text::getAlign() {
    return textAlign;
}

void Text::setX(int x) {
    image.setX(x);
}

void Text::setY(int y) {
    image.setY(y);
}

void Text::setString(const std::string &str) {
    this->str = str;
}

void Text::setAlign(Font::textAlign textAlign) {
    this->textAlign = textAlign;
}


void Text::setWrappingWidth(unsigned int width) {
    wrappingWidth = width;
}

void Text::setOverflowHeight(unsigned int height) {
    overflowHeight = height;
}

void Text::setLineSpacingMod(float mod) {
    lineSpacingMod = mod;
}

unsigned int Text::getWrappingWidth() {
    return wrappingWidth;
}

float Text::getLineSpacingMod() {
    return lineSpacingMod;
}

void Text::setFontSize(unsigned int size) {
    fontSize = size;
}
unsigned int Text::getFontSize() {
    return fontSize;
}

unsigned int Text::getWidth() {
    return image.getWidth();
}

unsigned int Text::getHeight() {
    return image.getHeight();
}
