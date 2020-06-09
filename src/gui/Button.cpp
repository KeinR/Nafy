#include "Button.h"

#include <iostream> // DEBUG
#include <cmath>

#include "../core/glfw.h"

#include "../env/env.h"

// TODO: This class is a fucking mess and needs serious work

// So that different buttons can communicate about the proper status of the cursor
static int mouseLock = 0;
static void grabHand() {
    if (mouseLock == 0) {
        nafy::setCursorType(nafy::cursorType::HAND);
    }
    mouseLock++;
}
static void releaseHand() {
    mouseLock--;
    if (mouseLock <= 0) {
        mouseLock = 0;
        nafy::setCursorType(nafy::cursorType::DEFAULT);
    }
}

template<class T>
void nafy::ButtonBase<T>::init() {
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

template<class T>
nafy::ButtonBase<T>::ButtonBase() {
    init();
}

template<class T>
nafy::ButtonBase<T>::ButtonBase(const Font::type &textFont, const shader_t &textShader, const shader_t &shapeShader):
    display(TextCrawl(textFont, textShader), Rectangle(shapeShader)) {

    init();
}
template<class T>
nafy::ButtonBase<T>::~ButtonBase() {
    getContext()->removeMousePosCallback(this);
    getContext()->removeMouseButtonCallback(this);
}

template<class T>
void nafy::ButtonBase<T>::updateNodesY() {
    Rectangle &box = display.getBox();

    // // Top left
    // nodes[0].y = display.getY() + box.getCornerRadiusTopLeft();

    // downmost = display.getY() + display.getHeight();

    // // Bottom left
    // nodes[1].y = downmost - box.getCornerRadiusBottomLeft();

    // // Top right
    // nodes[2].y = display.getY() + box.getCornerRadiusTopRight();

    // // Bottom right
    // nodes[3].y = downmost - box.getCornerRadiusBottomRight();

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

template<class T>
void nafy::ButtonBase<T>::updateNodesX() {
    Rectangle &box = display.getBox();

    // // Top left
    // nodes[0].x = display.getX() + box.getCornerRadiusTopLeft();

    // // Bottom left
    // nodes[1].x = display.getX() + box.getCornerRadiusBottomLeft();

    // rightmost = display.getX() + display.getWidth();

    // // Top right
    // nodes[2].x = rightmost - box.getCornerRadiusTopRight();

    // // Bottom right
    // nodes[3].x = rightmost - box.getCornerRadiusBottomRight();

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

template<class T>
void nafy::ButtonBase<T>::setX(int value) {
    display.setX(value);
    updateNodesX();
}
template<class T>
void nafy::ButtonBase<T>::setY(int value) {
    display.setX(value);
    updateNodesY();
}
template<class T>
void nafy::ButtonBase<T>::setWidth(unsigned int value) {
    display.setWidth(value);
    updateNodesX();
}
template<class T>
void nafy::ButtonBase<T>::setHeight(unsigned int value) {
    display.setHeight(value);
    updateNodesY();
}
template<class T>
void nafy::ButtonBase<T>::setMargin(unsigned int value) {
    display.setMargin(value);
    updateNodesX();
    updateNodesY();
}
template<class T>
void nafy::ButtonBase<T>::setCornerRadius(unsigned int value) {
    display.setCornerRadius(value);
    updateNodesX();
    updateNodesY();
}

template<class T>
void nafy::ButtonBase<T>::setEnabled(bool value) {
    if (enabled && !value && hovering) {
        hovering = false;
        releaseHand();
    }
    enabled = value;
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
    display.generate();
}
template<class T>
void nafy::ButtonBase<T>::render() {
    display.render();
}

template<class T>
bool nafy::ButtonBase<T>::containPoint(double xPos, double yPos) {
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

template<class T>
void nafy::ButtonBase<T>::mouseMoved(double mouseX, double mouseY) {
    if (!enabled) return;
    const bool inside = containPoint(mouseX, mouseY);
    if (hovering) {
        if (!inside) {
            hovering = false;
            releaseHand();
            if (onLeave) {
                onLeave();
            }
        }
    } else if (inside) {
        hovering = true;
        grabHand();
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
T &nafy::ButtonBase<T>::getDisplay() {
    return display;
}



template class nafy::ButtonBase<nafy::TextRec>;
template class nafy::ButtonBase<nafy::TextRecCrawl>;
