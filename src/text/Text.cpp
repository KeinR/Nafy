#include "Text.h"

#include <iostream>
#include <cmath>

#include "textdefs.h"

#include GLFW_INCLUDE_HEADER_LOCATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Text::generateBuffers() {
    float vertices[] = {
        // positions   // texture coords
        1.0,   1.0,  1.0f, 1.0f, // top right
        1.0,  -1.0,  1.0f, 0.0f, // bottom right
        -1.0, -1.0,  0.0f, 0.0f, // bottom left
        -1.0,  1.0,  0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VA);
    glGenBuffers(1, &VB);
    glGenBuffers(1, &EB);
    glGenTextures(1, &TX);

    glBindTexture(GL_TEXTURE_2D, TX);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // We don't plan on scaling this one, nearest is fine~
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindVertexArray(VA);

    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    constexpr int stride = 4 * sizeof(float);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Text::Text():
    face(nullptr), modelLocation(0),
    renderedWidth(0), renderedHeight(0),
    color{0, 0, 0, 1}, x(0), y(0),
    wrappingWidth(0), overflowHeight(0),
    lineSpacingMod(1.0f), shader(0), stopsIndex(0) {

    generateBuffers();
}

Text::Text(Face &face, const unsigned int shader):
    face(&face), modelLocation(glGetUniformLocation(shader, SHADER_MODEL)),
    renderedWidth(0), renderedHeight(0),
    color{0, 0, 0, 1}, x(0), y(0),
    wrappingWidth(0), overflowHeight(0),
    lineSpacingMod(1.0f), shader(shader), stopsIndex(0) {

    generateBuffers();

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, SHADER_TEXT_SAMPLER), 0);
}

// Memory manegemet

void Text::textSteal(Text &other) {
    TX = other.TX;
    // https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glDeleteTextures.xml
    // "glDeleteTextures silently ignores 0's"
    other.TX = 0;

    str = std::move(other.str);
    stops = std::move(other.stops);

    textCopyIL(other);
    textCopyPOD(other);
}

void Text::textCopy(const Text &other) {

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
    for (Face::line &ln : lines) {
        ln.start = index.begin() + (ln.start - other.index.begin());
        ln.end = index.begin() + (ln.end - other.index.begin());
    }
    for (Face::line_iterator &it : stops) {
        it = lines.begin() + (it - other.lines.begin());
    }
}

void Text::textCopyPOD(const Text &other) {
    face = other.face;

    renderedWidth = other.renderedWidth;
    renderedHeight = other.renderedHeight;

    for (int i = 0; i < 4; i++) {
        color[i] = other.color[i];
    }

    x = other.x;
    y = other.y;
    wrappingWidth = other.wrappingWidth;
    overflowHeight = other.overflowHeight;
    lineSpacingMod = other.lineSpacingMod;
    stopsIndex = other.stopsIndex;
}

Text::Text(const Text &other) {
    textCopy(other);
    loadStops();
}

Text::Text(Text &&other) {
    textSteal(other);
}

Text &Text::operator=(const Text &other) {
    textCopy(other);
    loadStops();
    return *this;
}

Text &Text::operator=(Text &&other) {
    glDeleteTextures(1, &TX);
    textSteal(other);
    return *this;
}

// End memory manegemet

Text::~Text() {
    glDeleteVertexArrays(1, &VA);
    glDeleteBuffers(1, &VB);
    glDeleteBuffers(1, &EB);
    glDeleteTextures(1, &TX);
}

void Text::bindShader(const unsigned int shader) {
    this->shader = shader;
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, SHADER_TEXT_SAMPLER), 0);
    modelLocation = glGetUniformLocation(shader, SHADER_MODEL);
}

void Text::generate() {
    if (!str.size()) {
        return;
    }

    index = face->indexString(str.cbegin(), str.cend());
    if (wrappingWidth) {
        lines = face->getLines(index.cbegin(), index.cend(), wrappingWidth);
        if (overflowHeight) {
            stops = face->breakupLines(lines.cbegin(), lines.cend(), lineSpacingMod, overflowHeight);
        } else {
            stops.clear();
            stops.push_back(lines.cend());
        }
    } else {
        lines.clear();
        lines.push_back(face->makeLine(index.cbegin(), index.cend()));
        stops.clear();
        stops.push_back(lines.cend());
    }

    loadLines(lines.cbegin(), stops[0]);
}

void Text::loadLines(const Face::line_iterator &start, const Face::line_iterator &end) {
    unsigned char *bitmap = face->renderLines(start, end, 4, color, lineSpacingMod, renderedWidth, renderedHeight);

    setTexture(bitmap);

    delete[] bitmap;
}

void Text::setTexture(unsigned char *bitmap) {
    glBindTexture(GL_TEXTURE_2D, TX);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderedWidth, renderedHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
}

void Text::loadStops() {
    Face::line_iterator start;
    if (stopsIndex != 0) {
        start = stops[stopsIndex - 1];
    } else {
        start = lines.begin();
    }
    loadLines(start, stops[stopsIndex]);
}

void Text::render() {
    glm::mat4 model(1.0f);

    const float xPos = normX(x + 0.375f + (float)renderedWidth / 2);
    const float yPos = normY(y + 0.375f + (float)renderedHeight / 2);

    int winWidth, winHeight;
    getWindowSize(&winWidth, &winHeight);
    model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
    model = glm::scale(model, glm::vec3((float)renderedWidth / winWidth, (float)renderedHeight / winHeight, 0.0f));

    glUseProgram(shader);

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VA);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
    glBindTexture(GL_TEXTURE_2D, TX);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool Text::nextOverflow() {
    if (stopsIndex + 1 < overflowSize()) {
        stopsIndex++;
        loadStops();
        return true;
    }
    return false;
}

Face::line_str::size_type Text::overflowSize() {
    return stops.size();
}

void Text::seekOverflow(Face::line_str::size_type i) {
    if (i < overflowSize()) {
        stopsIndex = i;
        loadStops();
    }
}

/* Setter bloat */


void Text::setColorHex(unsigned int hex) {
    const unsigned char r = hex >> 24;
    if (r) {
        setColorVal(
            r,
            (hex >> 16) & 0xFF,
            (hex >> 8) & 0xFF,
            hex & 0xFF
        );
    } else {
        setColorVal(
            (hex >> 16) & 0xFF,
            (hex >> 8) & 0xFF,
            hex & 0xFF,
            0xFF
        );
    }
}

void Text::setColorVal(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
    setColorProp(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

void Text::setColorProp(float red, float green, float blue, float alpha) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;
}

void Text::getColorVal(unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *alpha) {
    #define TRYGET(var, i) if (var != nullptr) *var = color[i] * 0xFF;
    TRYGET(red, 0)
    TRYGET(green, 1)
    TRYGET(blue, 2)
    TRYGET(alpha, 3)
    #undef TRYGET
}

Face *Text::getFace() {
    return face;
}

void Text::setFace(Face &face) {
    this->face = &face;
}

int Text::getX() {
    return x;
}

int Text::getY() {
    return y;
}

std::string &Text::getString() {
    return str;
}

void Text::setX(int x) {
    this->x = x;
}

void Text::setY(int y) {
    this->y = y;
}

void Text::setString(const std::string &str) {
    this->str = str;
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

int Text::getWidth() {
    return renderedWidth;
}

int Text::getHeight() {
    return renderedHeight;
}
