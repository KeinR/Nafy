#include "Button.h"

#include <iostream> // DEBUG
#include <cmath>

#include "../core/glfw.h"

#include "../env/env.h"

template<class T, class S>
void nafy::ButtonBase<T,S>::init() {
    enabled = true;
    hovering = false;
    setCornerRadius(0);
    getContext()->addMousePosCallback(*this);
    getContext()->addMouseButtonCallback(*this);
    setWidth(100);
    setHeight(50);
    updateNodesX();
    updateNodesY();
    display.getText().setAlign(Font::textAlign::center);
}

template<class T, class S>
nafy::ButtonBase<T,S>::ButtonBase() {
    init();
}

template<class T, class S>
nafy::ButtonBase<T,S>::ButtonBase(const Font::type &textFont, const shader_t &textShader, const shader_t &shapeShader):
    display(TextCrawl(textFont, textShader), Rectangle(shapeShader)) {

    init();
}
template<class T, class S>
nafy::ButtonBase<T,S>::~ButtonBase() {
    getContext()->removeMousePosCallback(this);
    getContext()->removeMouseButtonCallback(this);
}

template<class T, class S>
void nafy::ButtonBase<T,S>::updateNodesY() {
    Rectangle &box = display.getBox();

    // Top left
    nodes[0].y = display.getY() + box.getCornerRadiusTopLeft();

    // Top right
    nodes[1].y = display.getY() + box.getCornerRadiusTopRight();

    downmost = display.getY() + display.getHeight();

    // Bottom right
    nodes[2].y = downmost - box.getCornerRadiusBottomRight();

    // Bottom left
    nodes[3].y = downmost - box.getCornerRadiusBottomLeft();
}

template<class T, class S>
void nafy::ButtonBase<T,S>::updateNodesX() {
    Rectangle &box = display.getBox();

    // Top left
    nodes[0].x = display.getX() + box.getCornerRadiusTopLeft();

    rightmost = display.getX() + display.getWidth();

    // Top right
    nodes[1].x = rightmost - box.getCornerRadiusTopRight();

    // Bottom right
    nodes[2].x = rightmost - box.getCornerRadiusBottomRight();

    // Bottom left
    nodes[3].x = display.getX() + box.getCornerRadiusBottomLeft();

}

template<class T, class S>
void nafy::ButtonBase<T,S>::updateNodes() {
    updateNodesX();
    updateNodesY();
}

template<class T, class S>
void nafy::ButtonBase<T,S>::setOnClick(const press_callback_func &callback) {
    onClick = callback;
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setOnRelease(const press_callback_func &callback) {
    onRelease = callback;
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setOnEnter(const move_callback_func &callback) {
    onEnter = callback;
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setOnLeave(const move_callback_func &callback) {
    onLeave = callback;
}

template<class T, class S>
void nafy::ButtonBase<T,S>::setX(int value) {
    display.setX(value);
    updateNodesX();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setY(int value) {
    display.setY(value);
    updateNodesY();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setWidth(unsigned int value) {
    display.setWidth(value);
    updateNodesX();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setHeight(unsigned int value) {
    display.setHeight(value);
    updateNodesY();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setMargin(unsigned int value) {
    display.setMargin(value);
    updateNodes();
}

template<class T, class S>
void nafy::ButtonBase<T,S>::setCornerRadius(unsigned int radius) {
    display.setCornerRadius(radius);
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setCornerRadius(int index, unsigned int radius) {
    display.setCornerRadius(index, radius);
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setCornerRadiusTopLeft(unsigned int radius) {
    display.setCornerRadiusTopLeft(radius);
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setCornerRadiusTopRight(unsigned int radius) {
    display.setCornerRadiusTopRight(radius);
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setCornerRadiusBottomRight(unsigned int radius) {
    display.setCornerRadiusBottomRight(radius);
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setCornerRadiusBottomLeft(unsigned int radius) {
    display.setCornerRadiusBottomLeft(radius);
    updateNodes();
}

template<class T, class S>
void nafy::ButtonBase<T,S>::setEnabled(bool value) {
    if (enabled && !value && hovering) {
        hovering = false;
    }
    enabled = value;
}

template<class T, class S>
nafy::Color &nafy::ButtonBase<T,S>::getColor() {
    return display.getColor();
}

template<class T, class S>
void nafy::ButtonBase<T,S>::trigger() {
    if (onClick) {
        onClick(GLFW_MOUSE_BUTTON_LEFT, 0);
    }
    if (onRelease) {
        onRelease(GLFW_MOUSE_BUTTON_LEFT, 0);
    }
}

template<class T, class S>
void nafy::ButtonBase<T,S>::generate() {
    display.generate();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::render() {
    display.render();
}

template<class T, class S>
bool nafy::ButtonBase<T,S>::containPoint(double xPos, double yPos) {
    // More likely the mouse is somewhere here
    if ((xPos >= nodes[0].x && xPos <= nodes[2].x && yPos >= display.getY() && yPos <= downmost) || // Big center
        (xPos >= display.getX() && xPos <= rightmost && yPos >= nodes[0].y && yPos <= nodes[2].y) // Rotate that center
        ) {
        return true;
    }
    for (int i = 0; i < 4; i++) {
        if (std::hypot(xPos - nodes[i].x, yPos - nodes[i].y) < display.getBox().getCornerRadius(i)) {
            return true;
        }
    }
    return false;
}

template<class T, class S>
void nafy::ButtonBase<T,S>::mouseMoved(double mouseX, double mouseY) {
    if (!enabled) return;
    const bool inside = containPoint(mouseX, mouseY);
    if (hovering) {
        if (!inside) {
            hovering = false;
            if (onLeave) {
                onLeave();
            }
        }
    } else if (inside) {
        hovering = true;
        if (onEnter) {
            onEnter();
        }
    }
}
template<class T, class S>
void nafy::ButtonBase<T,S>::mouseClicked(bool isPressed, int button, int mods) {
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

template<class T, class S>
nafy::Rectangle &nafy::ButtonBase<T,S>::getBox() {
    return display.getBox();
}
template<class T, class S>
S &nafy::ButtonBase<T,S>::getText() {
    return display.getText();
}

template<class T, class S>
T &nafy::ButtonBase<T,S>::getDisplay() {
    return display;
}



template class nafy::ButtonBase<nafy::TextRec, Text>;
template class nafy::ButtonBase<nafy::TextRecCrawl, TextCrawl>;
