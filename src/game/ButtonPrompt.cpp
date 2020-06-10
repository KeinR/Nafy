#include "ButtonPrompt.h"

#include <iostream>

#include "../env/env.h"

nafy::ButtonPrompt::elementData::elementData(const std::string &str, const callback_func &func): str(str), func(func) {
}

nafy::ButtonPrompt::ButtonPrompt(): ButtonPrompt({.5, .5, .2, 1}, 0, nullptr) {
}
nafy::ButtonPrompt::ButtonPrompt(const Color &color, unsigned int cornerRadius, View *gview):
    view(gview == nullptr ? &getContext()->getGameView() : gview), added(false), shouldStop(true) {
    setCornerRadius(cornerRadius);
    setSpacing(5);
    setWidth(100);
    setHeight(50);
    setMargin(5);
    setColor(color);
}
nafy::ButtonPrompt::~ButtonPrompt() {
    leaveView();
}

void nafy::ButtonPrompt::steal(ButtonPrompt &other) {
    copyPOD(other);
    setView(*other.view);
    color = std::move(other.color);
    colorLight = std::move(other.colorLight);
    elements = std::move(other.elements);
    if (other.added) {
        joinView();
    }
}
void nafy::ButtonPrompt::copy(const ButtonPrompt &other) {
    copyPOD(other);
    setView(*other.view);
    color = other.color;
    colorLight = other.colorLight;
    elements = other.elements;
    if (other.added) {
        joinView();
    }
}
void nafy::ButtonPrompt::copyPOD(const ButtonPrompt &other) {
    cornerRadius = other.cornerRadius;
    spacing = other.spacing;
    width = other.width;
    height = other.height;
    shouldStop = other.shouldStop;
}

nafy::ButtonPrompt::ButtonPrompt(ButtonPrompt &&other) {
    added = false;
    steal(other);
}
nafy::ButtonPrompt::ButtonPrompt(const ButtonPrompt &other) {
    added = false;
    copy(other);
}
nafy::ButtonPrompt &nafy::ButtonPrompt::operator=(ButtonPrompt &&other) {
    steal(other);
    return *this;
}
nafy::ButtonPrompt &nafy::ButtonPrompt::operator=(const ButtonPrompt &other) {
    copy(other);
    return *this;
}

void nafy::ButtonPrompt::leaveView() {
    if (added) {
        added = false;
        view->remove(this);
    }
}

void nafy::ButtonPrompt::joinView() {
    if (!added) {
        view->add(this);
        added = true;
    }
}

void nafy::ButtonPrompt::activate() {
    joinView();
    for (Button &val : elements) {
        val.setEnabled(true);
    }
    shouldStop = false;
}
void nafy::ButtonPrompt::deactivate() {
    leaveView();
    for (Button &val : elements) {
        val.setEnabled(false);
    }
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

void nafy::ButtonPrompt::setView(View &gview) {
    leaveView();
    view = &gview;
}
void nafy::ButtonPrompt::setCornerRadius(unsigned int cornerRadius) {
    this->cornerRadius = cornerRadius;
}
void nafy::ButtonPrompt::add(const std::string &str, const callback_func &func) {
    add(elementData(str, func));
}
void nafy::ButtonPrompt::add(const elementData &cb) {
    Button button;
    button.getText().setAlign(Font::textAlign::center);
    button.getText().setString(cb.str);
    // Take func by value because cb is likely on stack
    const callback_func &func = cb.func;
    button.setOnClick([this, func](Button *caller, int button, int mods)->void{
        releaseCursor();
        func();
        // Also, NB: This'll happen on a different thread
        deactivate();
    });
    button.setEnabled(false);
    elements.push_back(std::move(button));
}
void nafy::ButtonPrompt::generate() {
    int winWidth, winHeight;
    getWindowSize(&winWidth, &winHeight);
    const int jump = height + spacing; // Distance between each top-left corner
    int y = (winHeight - (elements.size() * jump - spacing)) / 2; // Integer div
    const int x = (winWidth - width) / 2; // Integer div

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
    return shouldStop;
}

void nafy::ButtonPrompt::render() {
    for (Button &val : elements) {
        val.render();
    }
}
