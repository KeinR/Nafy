#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <string>

#include "ftype.h"

#include "Font.h"

#include "../shaders/Shader.h" // DEVIATION
#include "../gui/Image.h" // DEVIATION
#include "../gui/Color.h" // DEVIATION
#include "../gui/renderable.h" // DEVIATION

class Text: public nafy::renderable /*DEVIATION*/ { 
protected:
    Font::type font;

    nafy::Image image;

    nafy::Color color;

    std::string str;
    Font::map_size wrappingWidth;
    Font::map_size overflowHeight;
    float lineSpacingMod;
    Font::textAlign textAlign;
    unsigned int fontSize;

    Font::glyph_str index;
    Font::line_str lines;
    std::vector<Font::line_iterator>::size_type stopsIndex;
    std::vector<Font::line_iterator> stops;

    void generateBuffers();

    virtual void loadLines(const Font::line_iterator &start, const Font::line_iterator &end);
    void loadStops();

    void textSteal(Text &other);
    void textCopy(const Text &other);
    // Configures the font so that it renders according
    // to this object's settings. Should be done before most every function call
    void configureFont();
    inline void textCopyIL(const Text &other);
    inline void textCopyPOD(const Text &other);
public:
    // If made with this, must be set equal to another or have the font and shader set
    // before calling ANY other methods, otherwise undefined behavior is invoked
    Text();
    Text(const Font::type &font, const nafy::shader_t &shader);
    Text(const Text &other);
    Text(Text &&other);
    Text &operator=(const Text &other);
    Text &operator=(Text &&other);

    Font::type getFont();
    void setFont(const Font::type &font);

    // Clear the text and have it display a blank.
    // Similar effect is achieved with empty string or string with only spaces and newlines
    virtual void clear();

    int getX();
    int getY();
    std::string &getString(); // Let me know if the reference return here is a bad idea
    Font::textAlign getAlign();

    void setX(int x);
    void setY(int y);
    void setString(const std::string &str);
    void setAlign(Font::textAlign textAlign);

    nafy::Color &getColor();

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

    void setFontSize(unsigned int size);
    unsigned int getFontSize();

    // The width and height of the current texture
    unsigned int getWidth();
    unsigned int getHeight();

    void bindShader(const nafy::shader_t &shader);
    nafy::shader_t getShader();

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
