#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <string>

#include "renderable.h"
#include "ShaderProgram.h"
#include "context.h"

#include "text/Text.h"

// TODO: Render monitors for shaders
// TODO: Memory managment

namespace nafy {
    class Button: public renderable, public mouseMoveCallback, public mouseClickCallback {
    public:
        typedef void(*callback_t)();
    private:
        Text innerText;
        Rectangle box;
        bool enabled;
        unsigned int margin;
        int x;
        int y;
        unsigned int width;
        unsigned int height;
        std::function<callback_t> callback;

        // Transient
        bool hovering;
        bool pressed;
    public:
        // Takes current context defaults
        // Button();
        // Alternatively, set the face 
        Button(Face &textFace, shader_t textShader, shader_t shapeShader);
        ~Button();
        void setCallback(const std::function<callback_t> &cb);
        void setX(int value);
        void setY(int value);
        void setWidth(unsigned int value);
        void setHeight(unsigned int value);
        void setMargin(unsigned int value);
        void setEnabled(bool val);
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
