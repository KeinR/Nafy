#ifndef STATIC_TEXT_H_INCLUDED
#define STATIC_TEXT_H_INCLUDED

#include <string>

#include "ftype.h"

#include "Face.h"
#include "Font.h"

class Text {
protected:
    Face *face;

    unsigned int VA; // Array
    unsigned int VB; // Buffer
    unsigned int EB; // Elem. buffer
    unsigned int TX; // Text texture
    unsigned int modelLocation; // In shader

    Face::map_size renderedWidth;
    Face::map_size renderedHeight;

    float color[4];
    int x;
    int y;
    std::string str;
    Face::map_size wrappingWidth;
    Face::map_size overflowHeight;
    float lineSpacingMod;

    Face::glyph_str index;
    Face::line_str lines;
    std::vector<Face::line_iterator>::size_type stopsIndex;
    std::vector<Face::line_iterator> stops;

    void generateBuffers();

    void setTexture(unsigned char *bitmap);
    virtual void loadLines(const Face::line_iterator &start, const Face::line_iterator &end);
    void loadStops();

    inline void textSteal(Text &other);
    inline void textCopy(Text &other);
    inline void textCopyIL(Text &other);
    inline void textCopyPOD(Text &other);
public:
    // If made with this, must be set equal to another or have the face and shader set
    // before calling ANY other methods, otherwise undefined behavior is invoked
    Text();
    Text(Face &face, const unsigned int shader);
    Text(const Text &other);
    Text(Text &&other);
    Text &operator=(const Text &other);
    Text &operator=(Text &&other);
    virtual ~Text();

    Face *getFace();
    void setFace(Face &face);

    int getX();
    int getY();
    std::string &getString(); // Let me know if the reference return here is a bad idea

    void setX(int x);
    void setY(int y);
    void setString(const std::string &str);

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
    Face::line_str::size_type overflowSize();
    void seekOverflow(Face::line_str::size_type i);

    void generate();
    // Doesn't set the shader itself; perhaps change that..?
    void render();
};

#endif
