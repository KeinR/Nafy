#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED

#include <string>

#include "Font.h"

namespace nafy {
    class Text {
        // Rendering
        uint32_t VA;
        uint32_t VB;
        uint32_t EB;
        uint32_t TX;
        uint32_t uniformMatLoc;

        // Configuration
        Font *font; // Always initialized
        int wrapWidth;
        int xPos;
        int yPos;
        float fontSize;
        std::string text;
        float red;
        float green;
        float blue;
        float alpha;

        // Transient
        int displayWidth;
        int displayHeight;
        bool imageValid = false;

        void generate();
    public:
        Text(Font *font, const uint32_t shader);
        ~Text();
        void setFont(Font *fnt);
        void setText(const std::string &str);
        void setFontSize(float size);
        void setX(int x);
        void setY(int y);
        void setWrapWidth(int w);
        // Ignores first byte
        void setHexColor(unsigned int hex);
        void setHexColorRGBA(unsigned int hex);
        void setColorVal(unsigned char r, unsigned char g, unsigned char b);
        // r, g, & b should not exceed 1 nor fall short of 0, unless you want wacky unpredictable text
        void setColor(float r, float g, float b);
        void setAlphaVal(unsigned char a);
        // same deal here
        void setAlpha(float a);
        void render();
    };
}

#endif
