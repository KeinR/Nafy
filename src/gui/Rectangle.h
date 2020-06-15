#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "renderable.h"
#include "../shaders/Shader.h"
#include "Color.h"
#include "../render/Buffer.h"
#include "../render/Model.h"

/*
* A rectangle, a basic primitive found in basically all drawing libs.
* It's what you'd expect, except it has the ability to have curved corners.
* Each corner has a "radius", and that radius determines how large the curve is.
* Because each corner can have a different radius, the rendering calculations are
* more heavy than usual. Therefore, there are checks (one) that can efficiently
* render a rectangle with no curved corners.
*/

// TODO: Memory managment
// TODO: Set image as opposed to uniform background color. Perhaps utilize shaders? Seperate buffers?

namespace nafy {
    class Rectangle: public renderable {
        // Renders vertex data
        Buffer buffer;
        // Determines where the rectangle gets rendered
        Model model;
        // The location of the color uniform in the shader
        uniform_t colorLocation;
        // The shader used to render the buffer
        shader_t shader;

        // Top left, top right, bottom right, bottom left
        // The radii of the rectangle's corners
        unsigned int cornerRadii[4];
        // The overall color of the rectangle
        Color color;
        // Generate data for `buffer` based on the width, height
        // and corner radii of the rectangle
        void generateBuffers();
        // Called if the cornerradii are all zero, generates buffer data efficiently
        void generateCurveless();
        void copy(const Rectangle &other);
        // Caps the given radius to min(width, height) / 2
        unsigned int capRadius(unsigned int radius);
        // Re-caps all radii, called when there's a change in width/height.
        // See capRadius(...) for more details.
        void recalRadius();
    public:
        // Default initialization.
        // Takes the default prim shader from the current context
        // and sets all corner radii to zero.
        // Default color is determined by Color's default.
        Rectangle();
        // Like the above, however specifying a shader to use.
        // Shader requirements: color & model
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
