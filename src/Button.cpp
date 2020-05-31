#include "Button.h"

#include <cmath>

#include "glfw.h"

nafy::Button::Button(Face &textFace, shader_t textShader, shader_t shapeShader):
    innerText(textFace, textShader), box(shapeShader), enabled(true),
    margin(5), x(0), y(0),
    hovering(false), pressed(false) {

    getContext()->addMousePosCallback(this);
    getContext()->addMouseButtonCallback(this);
    setWidth(100);
    setHeight(50);
}
nafy::Button::~Button() {
    getContext()->removeMousePosCallback(this);
    getContext()->removeMouseButtonCallback(this);
}
void nafy::Button::setCallback(const std::function<callback_t> &cb) {
    callback = cb;
}
void nafy::Button::setX(int value) {
    x = value;
}
void nafy::Button::setY(int value) {
    y = value;
}
void nafy::Button::setWidth(unsigned int value) {
    width = value;
    constexpr float distBase = 1 - std::sqrt(2) / 2.0f;
    move = margin + box.getCornerRadius() * distBase;
    box.setX(x-move);
    innerText.setWidth(width);
    box.setWidth(width + move * 2);
}
void nafy::Button::setHeight(unsigned int value) {
    height = value;
    box.setY(y-move);
    box.setHeight(height + move * 2);
    innerText.setHeight(height);
}
void nafy::Button::setMargin(unsigned int value) {
    margin = mgn;
}
void nafy::Button::setTextVisble(bool val) {

}
void nafy::Button::trigger() {
    if (callback) {
        callback();
    }
}
void nafy::Button::generate() {

}
void nafy::Button::render() {



    if (displayText) {
        glUseProgram();
        innerText.render();
    }
}

void nafy::Button::mouseMoved(double mouseX, double mouseY) {

}
void nafy::Button::mouseClicked(bool isPressed, int button, int mods) {

}

Rectangle &getBox() {

}
Text &getText() {

}
