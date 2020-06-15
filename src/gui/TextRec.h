#ifndef TEXTREC_H_INCLUDED
#define TEXTREC_H_INCLUDED

#include "../text/Text.h"
#include "../text/TextCrawl.h"

#include "Rectangle.h"
#include "renderable.h"

/*
* I am very creative with names, hence why when I made a class
* that combined text and a rectangle, I called it TextRec(tangle).
* Jokes aside, this is, as aforementioned, text emplaced atop a rectangle.
* It's a bit more than that though. The main purpose of the merge was to
* make sure that the text is wrapped inside it correctly, and that changes
* to margin (distance between text and rectangle) and corner radius don't cause
* the text to poke outside or look ill-positioned.
* The class is templated so as to support both types of Text: Text and TextCrawl
*/

namespace nafy {
    template<class T>
    class TextRecBase: public renderable {
    public:
        // The transformation performed upon the wrapped text.
        // The text position and size are based off of the base
        // rectangle, then `xy` is added to the x/y values, etc.
        struct move_t {
            int xy;
            int width;
            int height;
        };
    private:
        // The innter text.
        // This can be two types: Text or TextCrawl
        T text;
        // The wrapper rectangle
        Rectangle box;

        // Distance between the box and the text,
        // as shown by the the dashed and vertical lines:
        //   _____________
        //  |         |   |
        //  |--The quick--|
        //  |  brown fox  |
        //  |_____________|
        //
        unsigned int margin;

        // Instance of move_t for this class
        move_t move;

        // Calculate the move for the given radius
        // This would be (1 - std::sqrt(2) / 2.0f) * radius + margins,
        // or (1 - sin(45)) * radius + margin...
        // The distance between the curved radius and the current one
        // So, say we have a rectangle corner of radius 0 here:
        //   ______
        //   |
        //   |
        // 
        // Its radius is increased to some arbitrary value:
        //     _____
        //    /
        //   /
        //   |
        // 
        // It returns the distance indicated by the dash:
        //   _______
        //   |-/
        //   |/
        //   |
        // 
        // Plus the margin.
        // Otherwise, given the margin is zero and the text was left
        // aligned, text that would normally appear fine given a a
        // radius of zero:
        //    _______
        //   |The quick 
        //   |brown fox
        //   |
        //
        // ...would appear broken with anything greater than that.
        //     _____
        //   T/he quick
        //   /brown fox
        //   |
        // 
        // Hence why we must move it.
        // 
        int calcMove(unsigned int radius);
        // Calls calcMove(...) multible times to re-calculate
        // the values in move
        void regenMove();
        // Re-compute the position of the wrapped text object
        // based on the values in `move` and the wrapped Rectangle's
        // position and size values
        void reposText();
        // Calls regenMove() and reposText() after the
        // corner radius has changed.
        void afRadius();
        // Sets values to their defaults and
        // initializes the object
        void init();
    public:
        // Default construct the wrapped text and rectangle
        TextRecBase();
        // Take user-provided text and rectangle - copy
        TextRecBase(const T &text, const Rectangle &box);
        // Take user-provided text and rectangle - move
        TextRecBase(T &&text, Rectangle &&box);

        // Get a copy of the current move value
        move_t getMove();

        // Set/get the margin
        void setMargin(unsigned int value);
        unsigned int getMargin();

        // Set positional and size values
        void setX(int x);
        void setY(int y);
        void setWidth(unsigned int width);
        void setHeight(unsigned int height);

        // [[[[[[ This is so dumb (╯°□°)╯︵ ┻━┻ ]]]]]]
        // TODO: This is dunb. Just use one getter/setter
        // with an index. It doesn't matter if it's easier to debug,
        // it's an absolute pain.
        void setCornerRadius(unsigned int radius);
        // By index; going clockwise, starting from top-left inclusive
        void setCornerRadius(int index, unsigned int radius);
        // One corner at a time
        void setCornerRadiusTopLeft(unsigned int radius);
        void setCornerRadiusTopRight(unsigned int radius);
        void setCornerRadiusBottomRight(unsigned int radius);
        void setCornerRadiusBottomLeft(unsigned int radius);

        // Get positional and size values
        int getX();
        int getY();
        unsigned int getWidth();
        unsigned int getHeight();

        // Get corner radii
        void getCornerRadii(unsigned int data[4]);
        unsigned int getCornerRadius(int index);
        unsigned int getCornerRadiusTopLeft();
        unsigned int getCornerRadiusTopRight();
        unsigned int getCornerRadiusBottomRight();
        unsigned int getCornerRadiusBottomLeft();

        // Get refernce to color value
        Color &getColor();

        // Calls generate() on wrapped objects
        void generate();
        // Calls render() on wrapped objects (box then text)
        void render() override;

        // Get references to the wrapped objects
        Rectangle &getBox();
        T &getText();
    };

    // Typedefs so that code is cleaner
    typedef TextRecBase<TextCrawl> TextRecCrawl;
    typedef TextRecBase<Text> TextRec;
}

#endif
