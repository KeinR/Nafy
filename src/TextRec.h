#ifndef TEXTREC_H_INCLUDED
#define TEXTREC_H_INCLUDED

#include "text/Text.h"
#include "text/TextCrawl.h"

#include "Rectangle.h"
#include "renderable.h"

namespace nafy {
    template<class T>
    class TextRecBase: public renderable {
        T text;
        Rectangle box;

        unsigned int margin;
        int move;

        inline void regenMove();
        inline void reposText();
    public:
        TextRecBase();
        TextRecBase(const T &text, const Rectangle &box);

        int getMove();

        void setMargin(unsigned int value);
        unsigned int getMargin();

        void setX(int x);
        void setY(int y);
        void setWidth(unsigned int width);
        void setHeight(unsigned int height);
        void setCornerRadius(unsigned int radius);

        int getX();
        int getY();
        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getCornerRadius();

        void generate();
        void render() override;

        Rectangle &getBox();
        T &getText();
    };

    typedef TextRecBase<TextCrawl> TextRecCrawl;
    typedef TextRecBase<Text> TextRec;
}

#endif
