#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "renderable.h"
#include "ShaderProgram.h"
#include "Color.h"

namespace nafy {
    class Rectangle: public renderable {
        unsigned int VA; // Array
        unsigned int VB; // Buffer
        unsigned int EB; // Elem. buffer
        unsigned int modelLocation; // In shader
        unsigned int colorLocation; // In shader
        unsigned int countIndices;

        unsigned int width;
        unsigned int height;
        int x;
        int y;
        unsigned int cornerRadius;
        Color color;
        inline void generateBuffers();
        inline void generateCurveless();
    public:
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

        Color &getColor();
    };
}

#endif
