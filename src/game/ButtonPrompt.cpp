#include "ButtonPrompt.h"

#include <iostream>

#include "../env/env.h"

nafy::ButtonPrompt::elementData::elementData(const std::string &str, const callback_func &func): str(str), func(func) {
}

nafy::ButtonPrompt::ButtonPrompt(): ButtonPrompt({.5, .5, .2, 1}, 0, nullptr) {
}
nafy::ButtonPrompt::ButtonPrompt(const Color &color, unsigned int cornerRadius, EventDispatch *parent):
    dispatch(parent == nullptr ? getContext()->getGameDispatch() : *parent),
    shouldStop(true)
{
    dispatch.addRenderCallback(*this);
    setCornerRadius(cornerRadius);
    setSpacing(5);
    setWidth(100);
    setHeight(50);
    setMargin(5);
    setColor(color);
}

void nafy::ButtonPrompt::activate() {
    dispatch.setToggled(true);
    shouldStop = false;
}
void nafy::ButtonPrompt::deactivate() {
    dispatch.setToggled(false);
    shouldStop = true;
}

nafy::ButtonPrompt &nafy::ButtonPrompt::setSpacing(unsigned int value) {
    spacing = value;
    return *this;
}
nafy::ButtonPrompt &nafy::ButtonPrompt::setWidth(unsigned int value) {
    width = value;
    return *this;
}
nafy::ButtonPrompt &nafy::ButtonPrompt::setHeight(unsigned int value) {
    height = value;
    return *this;
}
nafy::ButtonPrompt &nafy::ButtonPrompt::setMargin(unsigned int value) {
    margin = value;
    return *this;
}

unsigned int nafy::ButtonPrompt::getSpacing() {
    return spacing;
}
unsigned int nafy::ButtonPrompt::getWidth() {
    return width;
}
unsigned int nafy::ButtonPrompt::getHeight() {
    return height;
}
unsigned int nafy::ButtonPrompt::getMargin() {
    return margin;
}

void nafy::ButtonPrompt::setColor(const Color &color) {
    this->color = color;
    colorLight = color.brighten(0.3);
}

void nafy::ButtonPrompt::setDispatch(EventDispatch &parent) {
    dispatch.setParent(parent);
}
void nafy::ButtonPrompt::setCornerRadius(unsigned int cornerRadius) {
    this->cornerRadius = cornerRadius;
}
void nafy::ButtonPrompt::add(const std::string &str, const callback_func &func) {
    add(elementData(str, func));
}
void nafy::ButtonPrompt::add(const elementData &cb) {
    Button button(dispatch);
    button.getText().setAlign(Font::textAlign::center);
    button.getText().setString(cb.str);
    // Take func by value because cb is likely on stack
    const callback_func &func = cb.func;
    button.setOnClick([this, func](Button *caller, int button, int mods)->void{
        releaseCursor();
        func();
        this->shouldStop = true;
    });
    elements.push_back(std::move(button));
}
void nafy::ButtonPrompt::generate() {
    int winWidth, winHeight;
    getWindowSize(&winWidth, &winHeight);
    const int jump = height + spacing; // Distance between each top-left corner
    // Starting y:
    // window height minus the net height of the buttons stacked on
    // top of each-other, divided by 2
    int y = (winHeight - (elements.size() * jump - spacing)) / 2;
    // x position of the buttons, calculated similarly to the starting y
    const int x = (winWidth - width) / 2;

    for (Button &button : elements) {
        button.getColor().set(color);
        button.setOnEnter([this](Button *caller)->void{
            caller->getColor().set(this->colorLight);
            setCursorHand();
        });
        button.setOnLeave([this](Button *caller)->void{
            caller->getColor().set(this->color);
            releaseCursor();
        });
        button.setX(x);
        button.setY(y);
        button.setWidth(width);
        button.setHeight(height);
        button.setMargin(margin);
        button.generate();

        y += jump;
    }
}

nafy::ButtonPrompt &nafy::ButtonPrompt::operator+(const elementData &cb) {
    add(cb);
    return *this;
}
nafy::ButtonPrompt &nafy::ButtonPrompt::operator+(com command) {
    if (command == com::done) {
        generate();
    }
    return *this;
}

nafy::ButtonPrompt::elements_cont_t &nafy::ButtonPrompt::getElements() {
    return elements;
}

void nafy::ButtonPrompt::init(Context *ctx, Scene *sc) {
    activate();
}
bool nafy::ButtonPrompt::action(Context *ctx, Scene *sc) {
    if (shouldStop) {
        deactivate();
        return true;
    }
    return false;
}

void nafy::ButtonPrompt::render() {
    for (Button &val : elements) {
        val.render();
    }
}
