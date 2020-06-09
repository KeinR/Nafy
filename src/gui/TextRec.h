#ifndef TEXTREC_H_INCLUDED
#define TEXTREC_H_INCLUDED

#include "../text/Text.h"
#include "../text/TextCrawl.h"

#include "Rectangle.h"
#include "renderable.h"

namespace nafy {
    template<class T>
    class TextRecBase: public renderable {
    public:
        struct move_t {
            int xy;
            int width;
            int height;
        };
    private:
        T text;
        Rectangle box;

        unsigned int margin;

        move_t move;

        int calcMove(unsigned int radius);
        void regenMove();
        void reposText();
        void afRadius();
    public:
        TextRecBase();
        TextRecBase(const T &text, const Rectangle &box);

        move_t getMove();

        void setMargin(unsigned int value);
        unsigned int getMargin();

        void setX(int x);
        void setY(int y);
        void setWidth(unsigned int width);
        void setHeight(unsigned int height);

        // [[[[[[ This is so dumb (╯°□°)╯︵ ┻━┻ ]]]]]]
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

        void generate();
        void render() override;

        Rectangle &getBox();
        T &getText();
    };

    typedef TextRecBase<TextCrawl> TextRecCrawl;
    typedef TextRecBase<Text> TextRec;
}

#endif
