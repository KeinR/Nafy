#include "Rectangle.h"

#include <iostream>
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

nafy::Rectangle::Rectangle(shader_t shader): model(0, 0, 100, 100), cornerRadius(0) {
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
    cornerRadius = other.cornerRadius;
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
    if (!cornerRadius) {
        generateCurveless();
        return;
    }

    const unsigned int cornerVerticies = CURVE_SMOOTHNESS * cornerRadius;

    const unsigned int verticesLength = (12 + cornerVerticies * 4) * 2;
    const unsigned int indicesLength = 18 + (cornerVerticies + 1) * 3 * 4;
    float *vertices = new float[verticesLength];
    unsigned int *indices = new unsigned int[indicesLength];

    const float marginX = (float)cornerRadius / model.width * 2;
    const float marginY = (float)cornerRadius / model.height * 2;

    // Going from the top-left inner-corner, moving clockwise...

    // Dip top-left
    vertices[0] = -1 + marginX;
    vertices[1] = 1 - marginY;
    // Top
    vertices[2] = -1 + marginX;
    vertices[3] = 1;
    vertices[4] = 1 - marginX;
    vertices[5] = 1;
    // Dip top-right
    vertices[6] = 1 - marginX;
    vertices[7] = 1 - marginY;
    // Right
    vertices[8] = 1;
    vertices[9] = 1 - marginY;
    vertices[10] = 1;
    vertices[11] = -1 + marginY;
    // Dip bottom-right
    vertices[12] = 1 - marginX;
    vertices[13] = -1 + marginY;
    // Bottom
    vertices[14] = -1 + marginX;
    vertices[15] = -1;
    vertices[16] = 1 - marginX;
    vertices[17] = -1;
    // Dip bottom-left
    vertices[18] = -1 + marginX;
    vertices[19] = -1 + marginY;
    // Left
    vertices[20] = -1;
    vertices[21] = 1 - marginY;
    vertices[22] = -1;
    vertices[23] = -1 + marginY;

    // Same direction

    // Big middle rectangle, takes up top and bottom section
    indices[0] = 2;
    indices[1] = 1;
    indices[2] = 8;
    indices[3] = 1;
    indices[4] = 7;
    indices[5] = 8;
    // Left side
    indices[6] = 11;
    indices[7] = 0;
    indices[8] = 9;
    indices[9] = 11;
    indices[10] = 10;
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
    for (unsigned int c = 0; c < 24; c += 6) {
        float *crn = vertices + c;
        const unsigned int center = c / 2;
        const float rotation = PI / 2 / cornerVerticies;
        float radians = start + rotation;

        vertices[vert] = crn[0] + marginX * std::cos(radians);
        vertices[vert + 1] = crn[1] + marginY * std::sin(radians);
        indices[ind++] = center + 1 >= 24 ? 0 : center + 1;
        indices[ind++] = vert / 2;
        indices[ind++] = center;
        vert += 2;

        int count = 0;
        for (unsigned int i = 1; i < cornerVerticies; i++) {

            vertices[vert] = crn[0] + marginX * std::cos(radians);
            vertices[vert + 1] = crn[1] + marginY * std::sin(radians);
            indices[ind++] = vert / 2 - 1;
            indices[ind++] = vert / 2;
            indices[ind++] = center;
            vert += 2;

            radians += rotation;
            count++;
        }
        indices[ind++] = vert / 2 - 1;
        indices[ind++] = center - 1 > 11 ? 11 : center - 1;
        indices[ind++] = center;

        start -= startInc;
    }

    buffer.setVerticies(verticesLength, vertices);
    buffer.setIndices(indicesLength, indices);

    delete[] vertices;
    delete[] indices;
}
void nafy::Rectangle::render() {
    glUseProgram(shader);

    model.set();

    glUniform4fv(colorLocation, 1, color.get());

    buffer.render();
}

void nafy::Rectangle::bindShader(shader_t shader) {
    this->shader = shader;
    model.bindShader(shader);
    colorLocation = glGetUniformLocation(shader, SHADER_COLOR_NAME);
}

void nafy::Rectangle::setX(int x) {
    model.x = x;
}
void nafy::Rectangle::setY(int y) {
    model.y = y;
}
void nafy::Rectangle::setWidth(unsigned int width) {
    model.width = width;
    setCornerRadius(cornerRadius);
}
void nafy::Rectangle::setHeight(unsigned int height) {
    model.height = height;
    setCornerRadius(cornerRadius);
}
void nafy::Rectangle::setCornerRadius(unsigned int radius) {
    cornerRadius = std::min(radius, std::min(model.width, model.height) / 2);
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
unsigned int nafy::Rectangle::getCornerRadius() {
    return cornerRadius;
}

nafy::Color &nafy::Rectangle::getColor() {
    return color;
}
