#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <string>

#include "ftype.h"

#include "Font.h"

#include "../renderable.h" // DEVIATION

class Text: public nafy::renderable /*DEVIATION*/ { 
protected:
    Font::type font;
    unsigned int shader;

    unsigned int VA; // Array
    unsigned int VB; // Buffer
    unsigned int EB; // Elem. buffer
    unsigned int TX; // Text texture
    unsigned int modelLocation; // In shader

    Font::map_size renderedWidth;
    Font::map_size renderedHeight;

    float color[4];
    int x;
    int y;
    std::string str;
    Font::map_size wrappingWidth;
    Font::map_size overflowHeight;
    float lineSpacingMod;
    Font::textAlign textAlign;

    Font::glyph_str index;
    Font::line_str lines;
    std::vector<Font::line_iterator>::size_type stopsIndex;
    std::vector<Font::line_iterator> stops;

    void generateBuffers();

    void setTexture(unsigned char *bitmap);
    virtual void loadLines(const Font::line_iterator &start, const Font::line_iterator &end);
    void loadStops();

    void textSteal(Text &other);
    void textCopy(const Text &other);
    inline void textCopyIL(const Text &other);
    inline void textCopyPOD(const Text &other);
public:
    // If made with this, must be set equal to another or have the font and shader set
    // before calling ANY other methods, otherwise undefined behavior is invoked
    Text();
    Text(const Font::type &font, const unsigned int shader);
    Text(const Text &other);
    Text(Text &&other);
    Text &operator=(const Text &other);
    Text &operator=(Text &&other);
    virtual ~Text();

    Font::type getFont();
    void setFont(const Font::type &font);


    int getX();
    int getY();
    std::string &getString(); // Let me know if the reference return here is a bad idea
    Font::textAlign getAlign();

    void setX(int x);
    void setY(int y);
    void setString(const std::string &str);
    void setAlign(Font::textAlign textAlign);

    // Hex value for RGBA, with first 8 bytes ignored,
    // next 8 red, then green, blue, and lastly, alpha
    // If the first 8 bits are empty, then the format is assumed to
    // be RGB, and those bits are ignored and the alpha set to 0xFF
    // To illustrate:
    // 0xFF32AC33 -> RGBA -> R=FF, G=32, B=AC, A=33
    // 0xFF32AC == 0x00FF32AC -> Only 3 bytes -> RGB -> R=FF, G=32, B=AC, A=FF
    void setColorHex(unsigned int hex);
    // Color 0-255
    void setColorVal(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
    // Proportions, 0-1 representing percent of 255
    void setColorProp(float red, float green, float blue, float alpha = 1.0f);

    // Any parameter as `nullptr` is ignored
    void getColorVal(unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *alpha);

    // The height at which the text will be truncated.
    // A value of 0 enables (infinite) width
    void setWrappingWidth(unsigned int width);
    // The height at which the text will be truncated.
    // A value of 0 disables truncation
    void setOverflowHeight(unsigned int height);
    // Line spacing 
    void setLineSpacingMod(float mod);
    unsigned int getWrappingWidth();
    unsigned int getOverflowHeight();
    float getLineSpacingMod();

    // The width and height of the current texture
    int getWidth();
    int getHeight();

    void bindShader(const unsigned int shader);

    // Returns false if reached the end of the overflows
    bool nextOverflow();
    // Includes the non-overflowing start
    Font::line_str::size_type overflowSize();
    void seekOverflow(Font::line_str::size_type i);

    void generate();
    // Doesn't set the shader itself; perhaps change that..?
    void render() override;
};

#endif
