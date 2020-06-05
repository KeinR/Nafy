#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "renderable.h"
#include "Shader.h"
#include "Color.h"

// TODO: Memory managment
// TODO: Set image as opposed to uniform background color. Perhaps utilize shaders? Seperate buffers?

namespace nafy {
    class Rectangle: public renderable {
        unsigned int VA; // Array
        unsigned int VB; // Buffer
        unsigned int EB; // Elem. buffer
        unsigned int modelLocation; // In shader
        unsigned int colorLocation; // In shader
        unsigned int countIndices;
        shader_t shader;

        unsigned int width;
        unsigned int height;
        int x;
        int y;
        unsigned int cornerRadius;
        Color color;
        inline void generateBuffers();
        inline void generateCurveless();
    public:
        // Takes context default prim shader
        Rectangle();
        Rectangle(shader_t shader);
        ~Rectangle();
        void generate();
        void render() override;

        void bindShader(shader_t shader);

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
