#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <string>
#include <functional>

#include "renderable.h"
#include "Shader.h"
#include "Rectangle.h"
#include "callback.h"

#include "text/Text.h"

// TODO: Memory manegment

namespace nafy {
    class Button: public renderable, public mouseMoveCallback, public mouseClickCallback {
    public:
        typedef void(move_callback_t)();
        typedef std::function<move_callback_t> move_callback_func;
        typedef void(press_callback_t)(int button, int mods);
        typedef std::function<press_callback_t> press_callback_func;
    private:
        Text innerText;
        Rectangle box;
        bool enabled;
        unsigned int margin;
        int x;
        int y;
        unsigned int width;
        unsigned int height;

        press_callback_func onClick;
        press_callback_func onRelease;
        move_callback_func onEnter;
        move_callback_func onLeave;

        // Transient
        bool hovering;
        bool pressed;
        inline void init();
        inline int getMove();
        inline void calX();
        inline void calY();
        inline void calWidth();
        inline void calHeight();
        inline bool containPoint(double xPos, double yPos);
    public:
        // Takes current context defaults
        Button();
        // Alternatively, set the font and shaders 
        Button(const Font::type &textFont, shader_t textShader, shader_t shapeShader);
        ~Button();

        void setOnClick(const press_callback_func &callback);
        void setOnRelease(const press_callback_func &callback);
        void setOnEnter(const move_callback_func &callback);
        void setOnLeave(const move_callback_func &callback);

        void setX(int value);
        void setY(int value);
        void setWidth(unsigned int value);
        void setHeight(unsigned int value);
        void setMargin(unsigned int value);
        void setEnabled(bool value);
        void trigger();
        void generate();
        void render() override;
    
        void mouseMoved(double mouseX, double mouseY) override;
        void mouseClicked(bool isPressed, int button, int mods) override;

        Rectangle &getBox();
        Text &getText();
    };
}

#endif
