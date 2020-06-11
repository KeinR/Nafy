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
        Buffer buffer;
        Model model;
        uniform_t colorLocation; // In shader
        shader_t shader;

        // Top left, top right, bottom right, bottom left
        unsigned int cornerRadii[4];
        Color color;
        void generateBuffers();
        void generateCurveless();
        void initVA();
        void copy(const Rectangle &other);
        unsigned int capRadius(unsigned int radius);
        void doSetSide(int indexA, int indexB, unsigned int radius);
        void recalRadius();
    public:
        // Takes context default prim shader
        Rectangle();
        Rectangle(const shader_t &shader);

        void generate();
        void render() override;

        void bindShader(const shader_t &shader);
        shader_t getShader();

        void setX(int x);
        void setY(int y);
        void setWidth(unsigned int width);
        void setHeight(unsigned int height);

        // All corners
        void setCornerRadius(unsigned int radius);
        // By index; going clockwise, starting from top-left inclusive
        void setCornerRadius(int index, unsigned int radius);
        // One corner at a time
        void setCornerRadiusTopLeft(unsigned int radius);
        void setCornerRadiusTopRight(unsigned int radius);
        void setCornerRadiusBottomRight(unsigned int radius);
        void setCornerRadiusBottomLeft(unsigned int radius);

        int getX();
        int getY();
        unsigned int getWidth();
        unsigned int getHeight();
        void getCornerRadii(unsigned int data[4]);
        unsigned int getCornerRadius(int index);
        unsigned int getCornerRadiusTopLeft();
        unsigned int getCornerRadiusTopRight();
        unsigned int getCornerRadiusBottomRight();
        unsigned int getCornerRadiusBottomLeft();

        Color &getColor();
    };
}

#endif
