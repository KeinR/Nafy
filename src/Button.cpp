#include "Button.h"

#include <iostream> // DEBUG
#include <cmath>

#include "glfw.h"

#include "env.h"

nafy::Button::Button():
    enabled(true),
    margin(5), x(0), y(0),
    hovering(false), pressed(false) {
    
    getContext()->addMousePosCallback(*this);
    getContext()->addMouseButtonCallback(*this);
    setWidth(100);
    setHeight(50);
}

nafy::Button::Button(const Font::type &textFont, shader_t textShader, shader_t shapeShader):
    innerText(textFont, textShader), box(shapeShader), enabled(true),
    margin(5), x(0), y(0),
    hovering(false), pressed(false) {

    getContext()->addMousePosCallback(*this);
    getContext()->addMouseButtonCallback(*this);
    setWidth(100);
    setHeight(50);
}
nafy::Button::~Button() {
    getContext()->removeMousePosCallback(this);
    getContext()->removeMouseButtonCallback(this);
}
int nafy::Button::getMove() {
    constexpr float distBase = 1 - std::sqrt(2) / 2.0f;
    // Will break with very big numbers, like 2^31 big
    return margin + box.getCornerRadius() * distBase;
}

void nafy::Button::setOnClick(const press_callback_func &callback) {
    onClick = callback;
}
void nafy::Button::setOnRelease(const press_callback_func &callback) {
    onRelease = callback;
}
void nafy::Button::setOnEnter(const move_callback_func &callback) {
    onEnter = callback;
}
void nafy::Button::setOnLeave(const move_callback_func &callback) {
    onLeave = callback;
}

// This works for now
void nafy::Button::calX() {
    box.setX(x);
    innerText.setX(x + getMove());
}
void nafy::Button::calY() {
    box.setY(y);
    innerText.setY(y + getMove());
}
void nafy::Button::calWidth() {
    const int mmove = getMove();
    innerText.setWrappingWidth(width - mmove * 2);
    box.setWidth(width);
    innerText.setX(x + mmove);
}
void nafy::Button::calHeight() {
    const int mmove = getMove();
    box.setHeight(height);
    innerText.setY(y + mmove);
}
void nafy::Button::setX(int value) {
    x = value;
    calX();
}
void nafy::Button::setY(int value) {
    y = value;
    calY();
}
void nafy::Button::setWidth(unsigned int value) {
    width = value;
    calWidth();
}
void nafy::Button::setHeight(unsigned int value) {
    height = value;
    calHeight();
}
void nafy::Button::setMargin(unsigned int value) {
    margin = value;
    calX();
    calY();
    calWidth();
    calHeight();
}
void nafy::Button::setEnabled(bool value) {
    enabled = value;
}
void nafy::Button::generate() {
    box.generate();
    innerText.generate();
}
void nafy::Button::render() {
    box.render();
    innerText.render();
}

bool nafy::Button::containPoint(double xPos, double yPos) {
    return xPos >= x && xPos <= x + width && yPos >= y && yPos <= y + height;
}

void nafy::Button::mouseMoved(double mouseX, double mouseY) {
    if (!enabled) return;
    const bool inside = containPoint(mouseX, mouseY);
    if (hovering) {
        if (!inside) {
            hovering = false;
            setCursorType(cursorType::DEFAULT);
            if (onLeave) {
                onLeave();
            }
        }
    } else if (inside) {
        hovering = true;
        setCursorType(cursorType::HAND);
        if (onEnter) {
            onEnter();
        }
    }
}
void nafy::Button::mouseClicked(bool isPressed, int button, int mods) {
    if (!enabled) return;
    double mouseX, mouseY;
    glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);
    if (containPoint(mouseX, mouseY)) {
        if (isPressed) {
            if (onClick) {
                onClick(button, mods);
            }
        } else if (onRelease) {
            onRelease(button, mods);
        }
    }
}

nafy::Rectangle &nafy::Button::getBox() {
    return box;
}
Text &nafy::Button::getText() {
    return innerText;
}
