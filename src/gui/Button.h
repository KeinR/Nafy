#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <string>
#include <functional>

#include "renderable.h"
#include "../shaders/Shader.h"
#include "Rectangle.h"
#include "../core/callback.h"
#include "TextRec.h"
#include "Color.h"

#include "../text/Text.h"
#include "../text/TextCrawl.h"

// TODO: Abstract this to where it's just detecting moust clicks

namespace nafy {
    template<class T, class S>
    class ButtonBase: public renderable, public mouseMoveCallback, public mouseClickCallback {
    public:
        typedef std::function<void()> move_callback_func;
        typedef std::function<void(int button, int mods)> press_callback_func;
    private:
        T display;

        bool enabled;

        struct vec2 {
            int x;
            int y;
        };
        vec2 nodes[4];
        int rightmost;
        int downmost;

        press_callback_func onClick;
        press_callback_func onRelease;
        move_callback_func onEnter;
        move_callback_func onLeave;

        // Transient
        bool hovering;

        void init();
        void updateNodesX();
        void updateNodesY();
        void updateNodes();
        bool containPoint(double xPos, double yPos);
    public:
        // Takes current context defaults
        ButtonBase();
        // Alternatively, set the font and shaders 
        ButtonBase(const Font::type &textFont, const shader_t &textShader, const shader_t &shapeShader);
        ~ButtonBase();

        void setOnClick(const press_callback_func &callback);
        void setOnRelease(const press_callback_func &callback);
        void setOnEnter(const move_callback_func &callback);
        void setOnLeave(const move_callback_func &callback);

        press_callback_func getOnClick();
        press_callback_func getOnRelease();
        move_callback_func getOnEnter();
        move_callback_func getOnLeave();

        void setX(int value);
        void setY(int value);
        void setWidth(unsigned int value);
        void setHeight(unsigned int value);
        void setMargin(unsigned int value);

        int getX();
        int getY();
        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getMargin();

        void setCornerRadius(unsigned int radius);
        // By index; going clockwise, starting from top-left inclusive
        void setCornerRadius(int index, unsigned int radius);
        // One corner at a time
        void setCornerRadiusTopLeft(unsigned int radius);
        void setCornerRadiusTopRight(unsigned int radius);
        void setCornerRadiusBottomRight(unsigned int radius);
        void setCornerRadiusBottomLeft(unsigned int radius);

        void getCornerRadii(unsigned int data[4]);
        unsigned int getCornerRadius(int index);
        unsigned int getCornerRadiusTopLeft();
        unsigned int getCornerRadiusTopRight();
        unsigned int getCornerRadiusBottomRight();
        unsigned int getCornerRadiusBottomLeft();

        void setEnabled(bool value);
        bool getEnabled(); // 

        void trigger();
        void generate();
        void render() override;
    
        void mouseMoved(double mouseX, double mouseY) override;
        void mouseClicked(bool isPressed, int button, int mods) override;

        // wrapped in display
        Color &getColor();
        Rectangle &getBox();
        S &getText();

        T &getDisplay();
    };

    typedef ButtonBase<TextRec, Text> Button;
    typedef ButtonBase<TextRecCrawl, TextCrawl> CrawlButton;
}

#endif
