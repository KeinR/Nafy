#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <string>
#include <functional>

#include "renderable.h"
#include "Shader.h"
#include "Rectangle.h"
#include "callback.h"
#include "TextRec.h"

#include "text/Text.h"
#include "text/TextCrawl.h"

// TODO: Memory manegment

namespace nafy {
    template<class T>
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

        inline void init();
        inline void updateNodesX();
        inline void updateNodesY();
        inline bool containPoint(double xPos, double yPos);
    public:
        // Takes current context defaults
        ButtonBase();
        // Alternatively, set the font and shaders 
        ButtonBase(const Font::type &textFont, shader_t textShader, shader_t shapeShader);
        ~ButtonBase();

        void setOnClick(const press_callback_func &callback);
        void setOnRelease(const press_callback_func &callback);
        void setOnEnter(const move_callback_func &callback);
        void setOnLeave(const move_callback_func &callback);

        void setX(int value);
        void setY(int value);
        void setWidth(unsigned int value);
        void setHeight(unsigned int value);
        void setMargin(unsigned int value);
        void setCornerRadius(unsigned int value);

        void setEnabled(bool value);

        void trigger();
        void generate();
        void render() override;
    
        void mouseMoved(double mouseX, double mouseY) override;
        void mouseClicked(bool isPressed, int button, int mods) override;

        T &getDisplay();
    };

    typedef ButtonBase<TextRec> Button;
    typedef ButtonBase<TextRecCrawl> CrawlButton;
}

#endif
