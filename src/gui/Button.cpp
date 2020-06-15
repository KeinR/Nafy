#include "Button.h"

#include <iostream> // DEBUG
#include <cmath>

#include "../core/glfw.h"

#include "../env/env.h"

template<class T, class S>
void nafy::ButtonBase<T,S>::init() {
    hovering = false;
    setCornerRadius(0);
    enable();
    setWidth(100);
    setHeight(50);
    updateNodes();
    // I think that button text is usually centered, right?
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
    // Ensure that there are no pointers to this Button anywhere
    disable();
}

template<class T, class S>
void nafy::ButtonBase<T,S>::steal(ButtonBase &other) {
    display = std::move(other.display);
    onClick = std::move(other.onClick);
    onRelease = std::move(other.onRelease);
    onEnter = std::move(other.onEnter);
    onLeave = std::move(other.onLeave);
    hovering = other.hovering;

    setEnabled(other.enabled);

    // Just to be safe - re-calculate the cache
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::copy(const ButtonBase &other) {
    display = other.display;
    onClick = other.onClick;
    onRelease = other.onRelease;
    onEnter = other.onEnter;
    onLeave = other.onLeave;
    hovering = other.hovering;

    setEnabled(other.enabled);

    updateNodes();
}

template<class T, class S>
nafy::ButtonBase<T,S>::ButtonBase(ButtonBase &&other) {
    enabled = false;
    steal(other);
}
template<class T, class S>
nafy::ButtonBase<T,S>::ButtonBase(const ButtonBase &other) {
    enabled = false;
    copy(other);
}
template<class T, class S>
typename
nafy::ButtonBase<T,S> &nafy::ButtonBase<T,S>::operator=(ButtonBase &&other) {
    steal(other);
    return *this;
}
template<class T, class S>
typename
nafy::ButtonBase<T,S> &nafy::ButtonBase<T,S>::operator=(const ButtonBase &other) {
    copy(other);
    return *this;
}

template<class T, class S>
void nafy::ButtonBase<T,S>::disable() {
    enabled = false;
    getContext()->removeMousePosCallback(this);
    getContext()->removeMouseButtonCallback(this);
}
template<class T, class S>
void nafy::ButtonBase<T,S>::enable() {
    getContext()->addMousePosCallback(*this);
    getContext()->addMouseButtonCallback(*this);
    enabled = true;
}

template<class T, class S>
void nafy::ButtonBase<T,S>::updateNodes() {
    // std::cout << "Called" << std::endl;
    Rectangle &box = display.getBox();

    // Top left
    nodes[0].x = display.getX() + box.getCornerRadiusTopLeft();
    nodes[0].y = display.getY() + box.getCornerRadiusTopLeft();

    // Rightmost bound of button
    const float rightmost = display.getX() + display.getWidth();
    // Lower bound of button
    const float downmost = display.getY() + display.getHeight();

    // Top right
    nodes[1].x = rightmost - box.getCornerRadiusTopRight();
    nodes[1].y = display.getY() + box.getCornerRadiusTopRight();

    // Bottom right
    nodes[2].x = rightmost - box.getCornerRadiusBottomRight();
    nodes[2].y = downmost - box.getCornerRadiusBottomRight();

    // Bottom left
    nodes[3].x = display.getX() + box.getCornerRadiusBottomLeft();
    nodes[3].y = downmost - box.getCornerRadiusBottomLeft();

    // Set radii
    for (int i = 0; i < 4; i++) {
        nodes[i].radius = display.getCornerRadius(i);
    }

    // Bounds, used to efficiently determine if the mouse is close enough to
    // warrent more intense calculations

    bounds.set(display.getX(), display.getY(), rightmost, downmost);

    // Update data for the inner polygons

    constexpr int pointsLen = 8;
    float points[pointsLen];

    // Center block
    for (int i = 0; i < 4; i++) {
        const int index = i * 2;
        points[index] = nodes[i].x;
        points[index+1] = nodes[i].y;
    }
    boxes[0] = Polygon(points, pointsLen);

    // Leftmost
    points[0] = display.getX();
    points[1] = nodes[0].y;
    points[2] = display.getX();
    points[3] = nodes[3].y;
    points[4] = nodes[3].x;
    points[5] = nodes[3].y;
    points[6] = nodes[0].x;
    points[7] = nodes[0].y;
    boxes[1] = Polygon(points, pointsLen);
    // Topmost
    points[0] = nodes[0].x;
    points[1] = nodes[0].y;
    points[2] = nodes[0].x;
    points[3] = display.getY();
    points[4] = nodes[1].x;
    points[5] = display.getY();
    points[6] = nodes[1].x;
    points[7] = nodes[1].y;
    boxes[2] = Polygon(points, pointsLen);
    // Rightmost
    points[0] = nodes[1].x;
    points[1] = nodes[1].y;
    points[2] = rightmost;
    points[3] = nodes[1].y;
    points[4] = rightmost;
    points[5] = nodes[2].y;
    points[6] = nodes[2].x;
    points[7] = nodes[2].y;
    boxes[3] = Polygon(points, pointsLen);
    // Bottommost
    points[0] = nodes[2].x;
    points[1] = nodes[2].y;
    points[2] = nodes[2].x;
    points[3] = downmost;
    points[4] = nodes[3].x;
    points[5] = downmost;
    points[6] = nodes[3].x;
    points[7] = nodes[3].y;
    boxes[4] = Polygon(points, pointsLen);
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
typename
nafy::ButtonBase<T,S>::press_callback_func nafy::ButtonBase<T,S>::getOnClick() {
    return onClick;
}
template<class T, class S>
typename
nafy::ButtonBase<T,S>::press_callback_func nafy::ButtonBase<T,S>::getOnRelease() {
    return onRelease;
}
template<class T, class S>
typename
nafy::ButtonBase<T,S>::move_callback_func nafy::ButtonBase<T,S>::getOnEnter() {
    return onEnter;
}
template<class T, class S>
typename
nafy::ButtonBase<T,S>::move_callback_func nafy::ButtonBase<T,S>::getOnLeave() {
    return onLeave;
}

template<class T, class S>
void nafy::ButtonBase<T,S>::setX(int value) {
    display.setX(value);
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setY(int value) {
    display.setY(value);
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setWidth(unsigned int value) {
    display.setWidth(value);
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setHeight(unsigned int value) {
    display.setHeight(value);
    updateNodes();
}
template<class T, class S>
void nafy::ButtonBase<T,S>::setMargin(unsigned int value) {
    display.setMargin(value);
    updateNodes();
}

template<class T, class S>
int nafy::ButtonBase<T,S>::getX() {
    return display.getX();
}
template<class T, class S>
int nafy::ButtonBase<T,S>::getY() {
    return display.getY();
}
template<class T, class S>
unsigned int nafy::ButtonBase<T,S>::getWidth() {
    return display.getWidth();
}
template<class T, class S>
unsigned int nafy::ButtonBase<T,S>::getHeight() {
    return display.getHeight();
}
template<class T, class S>
unsigned int nafy::ButtonBase<T,S>::getMargin() {
    return display.getMargin();
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
    // It'd be a waste to remove ourselves from
    // the Context's callback vector twice
    if (enabled != value) {
        if (enabled) {
            hovering = false;
            disable();
        } else {
            enable();
        }
    }
}

template<class T, class S>
bool nafy::ButtonBase<T,S>::isEnabled() {
    return enabled;
}

template<class T, class S>
bool nafy::ButtonBase<T,S>::isHovering() {
    return hovering;
}

template<class T, class S>
void nafy::ButtonBase<T,S>::getCornerRadii(unsigned int data[4]) {
    display.getCornerRadii(data);
}
template<class T, class S>
unsigned int nafy::ButtonBase<T,S>::getCornerRadius(int index) {
    return display.getCornerRadius(index);
}
template<class T, class S>
unsigned int nafy::ButtonBase<T,S>::getCornerRadiusTopLeft() {
    return display.getCornerRadiusTopLeft();
}
template<class T, class S>
unsigned int nafy::ButtonBase<T,S>::getCornerRadiusTopRight() {
    return display.getCornerRadiusTopRight();
}
template<class T, class S>
unsigned int nafy::ButtonBase<T,S>::getCornerRadiusBottomRight() {
    return display.getCornerRadiusBottomRight();
}
template<class T, class S>
unsigned int nafy::ButtonBase<T,S>::getCornerRadiusBottomLeft() {
    return display.getCornerRadiusBottomLeft();
}

template<class T, class S>
nafy::Color &nafy::ButtonBase<T,S>::getColor() {
    return display.getColor();
}

template<class T, class S>
void nafy::ButtonBase<T,S>::trigger() {
    // I think it's convincing enough
    if (onClick) {
        onClick(this, GLFW_MOUSE_BUTTON_LEFT, 0);
    }
    if (onRelease) {
        onRelease(this, GLFW_MOUSE_BUTTON_LEFT, 0);
    }
}

template<class T, class S>
void nafy::ButtonBase<T,S>::generate() {
    display.generate();
    // If the text is already centered horizontally, center it vertically as well
    if (display.getText().getAlign() == Font::textAlign::center) {
        display.getText().setY(getY() + (getHeight() - display.getText().getHeight()) / 2);
    }
}
template<class T, class S>
void nafy::ButtonBase<T,S>::render() {
    display.render();
}

template<class T, class S>
bool nafy::ButtonBase<T,S>::containPoint(double xPos, double yPos) {
    // Called many times, this is a focus of optimization

    // O(1)
    if (bounds.hasPoint(xPos, yPos)) {
        // O(5n + 4)

        // Circles
        // O(4)
        for (int i = 0; i < 4; i++) {
            // O(1)
            if (nodes[i].hasPoint(xPos, yPos)) {
                return true;
            }
        }

        // Polygons
        // O(5n)
        for (int i = 0; i < 5; i++) {
            // O(n)
            if (boxes[i].hasPoint(xPos, yPos)) {
                return true;
            }
        }
    }
    return false;
}

template<class T, class S>
void nafy::ButtonBase<T,S>::mouseMoved(double mouseX, double mouseY) {
    if (!enabled) return; // TODO: This is stupid!
    const bool inside = containPoint(mouseX, mouseY);
    if (hovering) {
        if (!inside) {
            hovering = false;
            if (onLeave) {
                onLeave(this);
            }
        }
    } else if (inside) {
        hovering = true;
        if (onEnter) {
            onEnter(this);
        }
    }
}
template<class T, class S>
void nafy::ButtonBase<T,S>::mouseClicked(bool isPressed, int button, int mods) {
    if (!enabled) return; // TODO: This is stupid!
    double mouseX, mouseY;
    glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);
    if (containPoint(mouseX, mouseY)) {
        if (isPressed) {
            if (onClick) {
                onClick(this, button, mods);
            }
        } else if (onRelease) {
            onRelease(this, button, mods);
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

// Generate templates
template class nafy::ButtonBase<nafy::TextRec, Text>;
template class nafy::ButtonBase<nafy::TextRecCrawl, TextCrawl>;
