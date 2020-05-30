#include "Rectangle.h"

#include <iostream>
#include <cmath>

#include "glfw.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "env.h"
#include "defs.h"

// Number of verticies per unit of circumference, floored
#define CURVE_SMOOTHNESS 5

void nafy::Rectangle::generateBuffers() {
    glGenVertexArrays(1, &VA);
    glGenBuffers(1, &VB);
    glGenBuffers(1, &EB);

    generateCurveless();

    glBindVertexArray(VA);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
nafy::Rectangle::~Rectangle() {
    glDeleteVertexArrays(1, &VA);
    glDeleteBuffers(1, &VB);
    glDeleteBuffers(1, &EB);
}

nafy::Rectangle::Rectangle(shader_t shader): width(100), height(100), x(0), y(0), cornerRadius(0) {
    generateBuffers();
    generateCurveless();
    bindShader(shader);
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


    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    countIndices = 6;
}
void nafy::Rectangle::generate() {
    if (!cornerRadius) {
        generateCurveless();
        return;
    }
    // generateCurveless();
    // return;

    const unsigned int cornerVerticies = CURVE_SMOOTHNESS * cornerRadius;

    const unsigned int verticesLength = (12 + cornerVerticies * 4) * 2;
    const unsigned int indicesLength = 18 + (cornerVerticies + 1) * 3 * 4;
    countIndices = indicesLength;
    float *vertices = new float[verticesLength];
    unsigned int *indices = new unsigned int[indicesLength];

    int winWidth, winHeight;
    getWindowSize(&winWidth, &winHeight);
    const float marginX = (float)cornerRadius / winWidth;
    const float marginY = (float)cornerRadius / winHeight;

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
    indices[0] = 1;
    indices[1] = 2;
    indices[2] = 7;
    indices[3] = 1;
    indices[4] = 8;
    indices[5] = 7;
    // Left side
    indices[6] = 11;
    indices[7] = 0;
    indices[8] = 9;
    indices[9] = 11;
    indices[10] = 10;
    indices[11] = 9;
    // Right side
    indices[12] = 4;
    indices[13] = 3;
    indices[14] = 6;
    indices[15] = 4;
    indices[16] = 5;
    indices[17] = 6;

    unsigned int ind = 18;
    unsigned int vert = 24;
    
    #define DO_SET(last)\
        vertices[vert] = crn[0] + marginY * std::sin(radians);\
        vertices[vert + 1] = crn[1] + marginX * std::cos(radians);\
        indices[ind++] = (last);\
        indices[ind++] = vert / 2;\
        indices[ind++] = center;\
        vert += 2;

    constexpr float startInc = PI / 2;
    float start = startInc;
    for (int c = 0; c < 24; c += 6) {
        float *crn = vertices + c;
        const int center = c / 2;
        const float rotation = PI / 2 / cornerVerticies;
        float radians = start + rotation;
        // vertices[vert] = crn[0] + marginY * std::sin(radians);
        // vertices[vert + 1] = crn[1] + marginX * std::cos(radians);
        // indices[ind++] = center - 1 < 0 ? 11 : center - 1;
        // indices[ind++] = vert / 2;
        // indices[ind++] = center;
        // vert += 2;
        DO_SET(center + 1 >= 24 ? 0 : center + 1);
        for (unsigned int i = 1; i < cornerVerticies; i++) {
            DO_SET(vert / 2 - 1);
            radians += rotation;
        }
        indices[ind++] = vert / 2 - 1;
        indices[ind++] = center - 1 < 0 ? 11 : center - 1;
        indices[ind++] = center;

        start -= startInc;
    }

    #undef DO_SET

    std::cout << "ind > : " << ((int)indicesLength - (int)ind) << std::endl;
    std::cout << "vert > : " << ((int)verticesLength - (int)vert) << std::endl;

    glBindVertexArray(VA); // Needed?
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, verticesLength * sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    std::cout << "Delete" << std::endl;

    delete[] vertices;
    delete[] indices;

    std::cout << "Done" << std::endl;
}
void nafy::Rectangle::render() {
    glm::mat4 model(1.0f);

    const float xPos = normX(x + 0.375f + (float)width / 2);
    const float yPos = normY(y + 0.375f + (float)height / 2);

    int winWidth, winHeight;
    getWindowSize(&winWidth, &winHeight);
    model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
    model = glm::scale(model, glm::vec3((float)width / winWidth, (float)height / winHeight, 0.0f));

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    // glGetError();
    // glUniform4fv(colorLocation, 1, color.get());
    glUniform4f(colorLocation, 1, 1, 0, 1);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        switch (error) {
            case GL_INVALID_VALUE: std::cout << "GL_INVALID_VALUE" << std::endl; break;
            case GL_INVALID_OPERATION: std::cout << "GL_INVALID_OPERATION" << std::endl; break;
        }
        std::cout << "GL ERROR: " << std::hex << error << std::dec << std::endl;
    } else {
        std::cout << "No errors...." << std::endl;
    }

    glBindVertexArray(VA);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);

    glDrawElements(GL_TRIANGLES, countIndices, GL_UNSIGNED_INT, 0);
}

void nafy::Rectangle::bindShader(shader_t shader) {
    modelLocation = glGetUniformLocation(shader, SHADER_MODEL_NAME);
    colorLocation = glGetUniformLocation(shader, SHADER_COLOR_NAME);
}

void nafy::Rectangle::setX(int x) {
    this->x = x;
}
void nafy::Rectangle::setY(int y) {
    this->y = y;
}
void nafy::Rectangle::setWidth(unsigned int width) {
    this->width = width;
}
void nafy::Rectangle::setHeight(unsigned int height) {
    this->height = height;
}
void nafy::Rectangle::setCornerRadius(unsigned int radius) {
    cornerRadius = radius;
}

nafy::Color &nafy::Rectangle::getColor() {
    return color;
}
