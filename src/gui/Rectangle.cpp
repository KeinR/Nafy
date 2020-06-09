#include "Rectangle.h"

#include <cmath>

#include "../core/glfw.h"

#include "../env/env.h"
#include "../env/defs.h"

// Number of verticies per unit of circumference, floored
#define CURVE_SMOOTHNESS 3

void nafy::Rectangle::generateBuffers() {
    generateCurveless();
    initVA();
}

void nafy::Rectangle::initVA() {
    buffer.bindArr();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

nafy::Rectangle::Rectangle(): Rectangle(getContext()->getDefaultPrimShader()) {
}

nafy::Rectangle::Rectangle(const shader_t &shader): model(0, 0, 100, 100), cornerRadii{0, 0, 0, 0} {
    generateBuffers();
    generateCurveless();
    bindShader(shader);
}

void nafy::Rectangle::copy(const Rectangle &other) {
    model = other.model;
    buffer = other.buffer.derive();
    initVA();
    colorLocation = other.colorLocation;
    shader = other.shader;
    cornerRadii[0] = other.cornerRadii[0];
    cornerRadii[1] = other.cornerRadii[1];
    cornerRadii[2] = other.cornerRadii[2];
    cornerRadii[3] = other.cornerRadii[3];
    color = other.color;
}

nafy::Rectangle::Rectangle(const Rectangle &other) {
    copy(other);
}

nafy::Rectangle &nafy::Rectangle::operator=(const Rectangle &other) {
    copy(other);
    return *this;
}

void nafy::Rectangle::generateCurveless() {
    float vertices[] = {
        // positions
        1.0,   1.0, // top right
        1.0,  -1.0, // bottom right
        -1.0, -1.0, // bottom left
        -1.0,  1.0, // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    buffer.setVerticies(sizeof(vertices) / sizeof(float), vertices);
    buffer.setIndices(sizeof(indices) / sizeof(unsigned int), indices);
}

// TODO: render curves as their own seperate entity so that width
// and height changes don't require a re-generate()
void nafy::Rectangle::generate() {
    if (!(cornerRadii[0] || cornerRadii[1] || cornerRadii[2] || cornerRadii[3])) {
        generateCurveless();
        return;
    }

    unsigned int cornerVerticies[4];
    unsigned int cornerVerticiesTotal = 0;
    for (int i = 0; i < 4; i++) {
        cornerVerticies[i] = CURVE_SMOOTHNESS * cornerRadii[i] + 1;
        cornerVerticiesTotal += cornerVerticies[i];
    }

    const unsigned int verticesLength = (12 + cornerVerticiesTotal) * 2;
    const unsigned int indicesLength = 18 + (cornerVerticiesTotal + 4) * 3;
    float *vertices = new float[verticesLength];
    unsigned int *indices = new unsigned int[indicesLength];

    struct vec2 {
        float x;
        float y;
    } margins[4];
    for (int i = 0; i < 4; i++) {
        margins[i].x = (float)cornerRadii[i] / model.width * 2;
        margins[i].y = (float)cornerRadii[i] / model.height * 2;
    }

    // I present: the bane of dyslexics

    // Going from the top-left inner-corner, moving clockwise...

    // Dip top-left
    vertices[0] = -1 + margins[0].x;
    vertices[1] = 1 - margins[0].y;
    // Top
    vertices[2] = -1 + margins[0].x;
    vertices[3] = 1;
    vertices[4] = 1 - margins[1].x;
    vertices[5] = 1;
    // Dip top-right
    vertices[6] = 1 - margins[1].x;
    vertices[7] = 1 - margins[1].y;
    // Right
    vertices[8] = 1;
    vertices[9] = 1 - margins[1].y;
    vertices[10] = 1;
    vertices[11] = -1 + margins[2].y;
    // Dip bottom-right
    vertices[12] = 1 - margins[2].x;
    vertices[13] = -1 + margins[2].y;
    // Bottom
    vertices[14] = 1 - margins[2].x;
    vertices[15] = -1;
    vertices[16] = -1 + margins[3].x;
    vertices[17] = -1;
    // Dip bottom-left
    vertices[18] = -1 + margins[3].x;
    vertices[19] = -1 + margins[3].y;
    // Left
    vertices[20] = -1;
    vertices[21] = -1 + margins[3].y;
    vertices[22] = -1;
    vertices[23] = 1 - margins[0].y;

    // Same direction

    // Big middle rectangle, takes up top and bottom section
    indices[0] = 2;
    indices[1] = 1;
    indices[2] = 8;
    indices[3] = 2;
    indices[4] = 7;
    indices[5] = 8;
    // Left side
    indices[6] = 10;
    indices[7] = 0;
    indices[8] = 9;
    indices[9] = 10;
    indices[10] = 11;
    indices[11] = 0;
    // Right side
    indices[12] = 4;
    indices[13] = 3;
    indices[14] = 6;
    indices[15] = 4;
    indices[16] = 5;
    indices[17] = 6;

    unsigned int ind = 18;
    unsigned int vert = 24;

    constexpr float startInc = PI / 2;
    float start = startInc;
    // Going clockwise from top-left
    for (unsigned int c = 0; c < 24; c += 6) {
        float *crn = vertices + c;
        const int ri = c / 6;
        const unsigned int center = c / 2;
        const float rotation = PI / 2 / cornerVerticies[ri];
        float radians = start + rotation;

        vertices[vert] = crn[0] + margins[ri].x * std::cos(radians);
        vertices[vert + 1] = crn[1] + margins[ri].y * std::sin(radians);
        indices[ind++] = center + 1 >= 24 ? 0 : center + 1;
        indices[ind++] = vert / 2;
        indices[ind++] = center;
        vert += 2;

        int count = 0;
        for (unsigned int i = 1; i < cornerVerticies[ri]; i++) {
            vertices[vert] = crn[0] + margins[ri].x * std::cos(radians);
            vertices[vert + 1] = crn[1] + margins[ri].y * std::sin(radians);
            indices[ind++] = vert / 2 - 1;
            indices[ind++] = vert / 2;
            indices[ind++] = center;
            vert += 2;

            radians += rotation;
            count++;
        }
        indices[ind++] = vert / 2 - 1;
        indices[ind++] = center - 1 > 11 ? 11 : center - 1; // > because it's unsigned
        indices[ind++] = center;

        start -= startInc;
    }

    buffer.setVerticies(verticesLength, vertices);
    buffer.setIndices(indicesLength, indices);

    delete[] vertices;
    delete[] indices;
}
void nafy::Rectangle::render() {
    shader->use();

    model.set();

    glUniform4fv(colorLocation, 1, color.get());

    buffer.render();
}

void nafy::Rectangle::bindShader(const shader_t &shader) {
    this->shader = shader;
    model.bindShader(shader);
    colorLocation = shader->uniColor();
}

nafy::shader_t nafy::Rectangle::getShader() {
    return shader;
}

void nafy::Rectangle::setX(int x) {
    model.x = x;
}
void nafy::Rectangle::setY(int y) {
    model.y = y;
}

void nafy::Rectangle::recalRadius() {
    for (int i = 0; i < 4; i++) {
        setCornerRadius(i, cornerRadii[i]);
    }
}
void nafy::Rectangle::setWidth(unsigned int width) {
    model.width = width;
    recalRadius();
}
void nafy::Rectangle::setHeight(unsigned int height) {
    model.height = height;
    recalRadius();
}

// Cap the radius so that the rendering doesn't break
unsigned int nafy::Rectangle::capRadius(unsigned int radius) {
    return std::min(radius, std::min(model.width, model.height) / 2);
}

void nafy::Rectangle::setCornerRadius(unsigned int radius) {
    const unsigned int cornerRad = capRadius(radius);
    for (int i = 0; i < 4; i++) {
        cornerRadii[i] = cornerRad;
    }
}

// I like to think of myself as a DRY person
void nafy::Rectangle::setCornerRadius(int index, unsigned int radius) {
    cornerRadii[index] = capRadius(radius);
}
void nafy::Rectangle::setCornerRadiusTopLeft(unsigned int radius) {
    setCornerRadius(0, radius);
}
void nafy::Rectangle::setCornerRadiusTopRight(unsigned int radius) {
    setCornerRadius(1, radius);
}
void nafy::Rectangle::setCornerRadiusBottomRight(unsigned int radius) {
    setCornerRadius(2, radius);
}
void nafy::Rectangle::setCornerRadiusBottomLeft(unsigned int radius) {
    setCornerRadius(3, radius);
}

int nafy::Rectangle::getX() {
    return model.x;
}
int nafy::Rectangle::getY() {
    return model.y;
}
unsigned int nafy::Rectangle::getWidth() {
    return model.width;
}
unsigned int nafy::Rectangle::getHeight() {
    return model.height;
}
void nafy::Rectangle::getCornerRadii(unsigned int data[4]) {
    for (int i = 0; i < 4; i++) {
        data[i] = cornerRadii[i];
    }
}
unsigned int nafy::Rectangle::getCornerRadius(int index) {
    return cornerRadii[index];
}
unsigned int nafy::Rectangle::getCornerRadiusTopLeft() {
    return cornerRadii[0];
}
unsigned int nafy::Rectangle::getCornerRadiusTopRight() {
    return cornerRadii[1];
}
unsigned int nafy::Rectangle::getCornerRadiusBottomRight() {
    return cornerRadii[2];
}
unsigned int nafy::Rectangle::getCornerRadiusBottomLeft() {
    return cornerRadii[3];
}

nafy::Color &nafy::Rectangle::getColor() {
    return color;
}
