#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "renderable.h"
#include "../shaders/Shader.h"
#include "Color.h"
#include "../render/Buffer.h"
#include "../render/Model.h"

// TODO: Memory managment
// TODO: Set image as opposed to uniform background color. Perhaps utilize shaders? Seperate buffers?

namespace nafy {
    class Rectangle: public renderable {
        Model model;
        Buffer buffer;
        uniform_t colorLocation; // In shader
        shader_t shader;

        unsigned int cornerRadius;
        Color color;
        void generateBuffers();
        void generateCurveless();
        void initVA();
        void copy(const Rectangle &other);
    public:
        // Takes context default prim shader
        Rectangle();
        Rectangle(const shader_t &shader);
        Rectangle(Rectangle &&other) = default;
        Rectangle(const Rectangle &other);
        ~Rectangle() = default;
        Rectangle &operator=(Rectangle &&other) = default;
        Rectangle &operator=(const Rectangle &other);
        void generate();
        void render() override;

        void bindShader(const shader_t &shader);

        void setX(int x);
        void setY(int y);
        void setWidth(unsigned int width);
        void setHeight(unsigned int height);
        // Heavy function - triggers a buffer re-generation and
        // computation of a bunch of verticies
        void setCornerRadius(unsigned int radius);

        int getX();
        int getY();
        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getCornerRadius();

        Color &getColor();
    };
}

#endif
