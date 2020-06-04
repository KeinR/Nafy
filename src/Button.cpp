#include "Button.h"

#include <iostream> // DEBUG
#include <cmath>

#include "glfw.h"

#include "env.h"

template<class T>
void nafy::ButtonBase<T>::init() {
    enabled = true;
    margin = 5;
    x = 0;
    y = 0;
    hovering = false;
    pressed = true;
    setCornerRadius(0);
    updateMove();
    getContext()->addMousePosCallback(*this);
    getContext()->addMouseButtonCallback(*this);
    setWidth(100);
    setHeight(50);
    updateNodesX();
    updateNodesY();
    innerText.setAlign(Font::textAlign::center);
}

template<class T>
nafy::ButtonBase<T>::ButtonBase() {
    init();
}

template<class T>
nafy::ButtonBase<T>::ButtonBase(const Font::type &textFont, shader_t textShader, shader_t shapeShader):
    innerText(textFont, textShader), box(shapeShader) {

    init();
}
template<class T>
nafy::ButtonBase<T>::~ButtonBase() {
    getContext()->removeMousePosCallback(this);
    getContext()->removeMouseButtonCallback(this);
}
template<class T>
void nafy::ButtonBase<T>::updateMove() {
    constexpr float distBase = 1 - std::sqrt(2) / 2.0f;
    // Will break with very big numbers, like 2^31 big
    move = margin + cornerRadius * distBase;
}

template<class T>
void nafy::ButtonBase<T>::updateNodesY() {
    // Top left
    nodes[0].y = y + cornerRadius;

    downmost = y + height;

    // Bottom left
    nodes[1].y = downmost - cornerRadius;

    // Top right
    nodes[2].y = y + cornerRadius;

    // Bottom right
    nodes[3].y = downmost - cornerRadius;
}

template<class T>
void nafy::ButtonBase<T>::updateNodesX() {
    // Top left
    nodes[0].x = x + cornerRadius;

    // Bottom left
    nodes[1].x = x + cornerRadius;

    rightmost = x + width;

    // Top right
    nodes[2].x = rightmost - cornerRadius;

    // Bottom right
    nodes[3].x = x + width - cornerRadius;

}

template<class T>
void nafy::ButtonBase<T>::setOnClick(const press_callback_func &callback) {
    onClick = callback;
}
template<class T>
void nafy::ButtonBase<T>::setOnRelease(const press_callback_func &callback) {
    onRelease = callback;
}
template<class T>
void nafy::ButtonBase<T>::setOnEnter(const move_callback_func &callback) {
    onEnter = callback;
}
template<class T>
void nafy::ButtonBase<T>::setOnLeave(const move_callback_func &callback) {
    onLeave = callback;
}

// This works for now
template<class T>
void nafy::ButtonBase<T>::calX() {
    box.setX(x);
    innerText.setX(x + move);
}
template<class T>
void nafy::ButtonBase<T>::calY() {
    box.setY(y);
    innerText.setY(y + move);
}
template<class T>
void nafy::ButtonBase<T>::calWidth() {
    innerText.setWrappingWidth(width - move * 2);
    box.setWidth(width);
    innerText.setX(x + move);
}
template<class T>
void nafy::ButtonBase<T>::calHeight() {
    box.setHeight(height);
    innerText.setY(y + move);
}
template<class T>
void nafy::ButtonBase<T>::calAll() {
    calX();
    calY();
    calWidth();
    calHeight();
    updateNodesX();
    updateNodesY();
}
template<class T>
void nafy::ButtonBase<T>::setX(int value) {
    x = value;
    calX();
    updateNodesX();
}
template<class T>
void nafy::ButtonBase<T>::setY(int value) {
    y = value;
    calY();
    updateNodesY();
}
template<class T>
void nafy::ButtonBase<T>::setWidth(unsigned int value) {
    width = value;
    calWidth();
    updateNodesX();
}
template<class T>
void nafy::ButtonBase<T>::setHeight(unsigned int value) {
    height = value;
    calHeight();
    updateNodesY();
}
template<class T>
void nafy::ButtonBase<T>::setMargin(unsigned int value) {
    margin = value;
    calAll();
}
template<class T>
void nafy::ButtonBase<T>::setEnabled(bool value) {
    enabled = value;
}

template<class T>
void nafy::ButtonBase<T>::setCornerRadius(unsigned int value) {
    cornerRadius = value;
    box.setCornerRadius(value);
    updateMove();
    updateNodesX();
    updateNodesY();
}

template<class T>
void nafy::ButtonBase<T>::trigger() {
    if (onClick) {
        onClick(GLFW_MOUSE_BUTTON_LEFT, 0);
    }
    if (onRelease) {
        onRelease(GLFW_MOUSE_BUTTON_LEFT, 0);
    }
}

template<class T>
void nafy::ButtonBase<T>::generate() {
    updateMove();
    calAll();
    box.generate();
    innerText.generate();
}
template<class T>
void nafy::ButtonBase<T>::render() {
    box.render();
    innerText.render();
}

template<class T>
bool nafy::ButtonBase<T>::containPoint(double xPos, double yPos) {
    // More likely the mouse is somewhere here
    if ((xPos >= nodes[0].x && xPos <= nodes[3].x && yPos >= y && yPos <= downmost) || // Big center
        (xPos >= x && xPos <= rightmost && yPos >= nodes[0].y && yPos <= nodes[3].y) // Rotate that center
        ) {
        return true;
    }
    for (int i = 0; i < 4; i++) {
        if (std::hypot(xPos - nodes[i].x, yPos - nodes[i].y) < cornerRadius) {
            return true;
        }
    }
    return false;
}

template<class T>
void nafy::ButtonBase<T>::mouseMoved(double mouseX, double mouseY) {
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
template<class T>
void nafy::ButtonBase<T>::mouseClicked(bool isPressed, int button, int mods) {
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

template<class T>
nafy::Rectangle &nafy::ButtonBase<T>::getBox() {
    return box;
}

template<class T>
T &nafy::ButtonBase<T>::getText() {
    return innerText;
}



template class nafy::ButtonBase<Text>;
template class nafy::ButtonBase<TextCrawl>;
