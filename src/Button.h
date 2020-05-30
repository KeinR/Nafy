#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <string>

#include "renderable.h"
#include "ShaderProgram.h"

#include "text/Text.h"

namespace nafy {
    class Button: public renderable {
        Text innerText;
        bool displayText;
    public:
        // Takes current context defaults
        Button();
        // Alternatively, set the face 
        Button(Face &font, shader_t shader);
        void setText(const std::string &str);
        void setWidth(unsigned int width);
        void setHeight(unsigned int height);
        void setTextVisble(bool val);
        void trigger();
        void render() override;
    };
}

#endif
