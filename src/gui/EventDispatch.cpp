#include "EventDispatch.h"

#include <iostream> // DEBUG
#include <unordered_map>

static constexpr bool defToggle = false;
static constexpr nafy::EventDispatch *defParent = nullptr;

typedef std::unordered_map<GLFWwindow*, nafy::EventDispatch*> dispatch_map_t;
typedef dispatch_map_t::const_iterator cb_location_t;

static dispatch_map_t callbacks;

void nafy::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    cb_location_t loc = callbacks.find(window);
    if (loc != callbacks.cend()) {
        loc->second->mouseClicked(action == GLFW_PRESS, button, mods);
    }
}

void nafy::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    cb_location_t loc = callbacks.find(window);
    if (loc != callbacks.cend()) {
        loc->second->mouseMoved(xpos, ypos);
    }
}

void nafy::keyActionCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    cb_location_t loc = callbacks.find(window);
    if (loc != callbacks.cend()) {
        loc->second->keyAction(key, scancode, action, mods);
    }
}

nafy::EventDispatch::EventDispatch(): toggled(defToggle), parent(defParent) {
    setToggled(toggled);
}
nafy::EventDispatch::EventDispatch(EventDispatch &parent): toggled(defToggle), parent(&parent) {
    setToggled(toggled);
}
nafy::EventDispatch::EventDispatch(bool toggled): toggled(defToggle), parent(defParent) {
    setToggled(toggled);
}
nafy::EventDispatch::EventDispatch(bool toggled, EventDispatch &parent): toggled(defToggle), parent(&parent) {
    setToggled(toggled);
}

nafy::EventDispatch::~EventDispatch() {
    unroot();
}

void nafy::EventDispatch::mouseMoved(double mouseX, double mouseY) {
    for (mouseMoveCallback *&callback : moveCallbacks) {
        callback->mouseMoved(mouseX, mouseY);
    }
}

void nafy::EventDispatch::mouseClicked(bool isPressed, int button, int mods) {
    for (mouseClickCallback *&callback : clickCallbacks) {
        callback->mouseClicked(isPressed, button, mods);
    }
}

void nafy::EventDispatch::keyAction(int key, int scancode, int action, int mods) {
    for (keyCallback *&callback : keyCallbacks) {
        callback->keyAction(key, scancode, action, mods);
    }
}

void nafy::EventDispatch::addMousePosCallback(mouseMoveCallback &callback) {
    moveCallbacks.push_back(&callback);
}

void nafy::EventDispatch::addMouseButtonCallback(mouseClickCallback &callback) {
    clickCallbacks.push_back(&callback);
}

void nafy::EventDispatch::addKeyCallback(keyCallback &callback) {
    keyCallbacks.push_back(&callback);
}

void nafy::EventDispatch::removeMousePosCallback(mouseMoveCallback *callback) {
    std::cout << "TRY CBEGIN" << std::endl;
    std::cout << this->toggled << std::endl;
    std::cout << "DONE" << std::endl;
    for (std::vector<mouseMoveCallback*>::const_iterator it = moveCallbacks.cbegin(); it != moveCallbacks.cend(); ++it) {
        if (*it == callback) {
            moveCallbacks.erase(it);
            break;
        }
    }
}

void nafy::EventDispatch::removeMouseButtonCallback(mouseClickCallback *callback) {
    for (std::vector<mouseClickCallback*>::const_iterator it = clickCallbacks.cbegin(); it != clickCallbacks.cend(); ++it) {
        if (*it == callback) {
            clickCallbacks.erase(it);
            break;
        }
    }
}

void nafy::EventDispatch::removeKeyCallback(keyCallback *callback) {
    for (std::vector<keyCallback*>::const_iterator it = keyCallbacks.cbegin(); it != keyCallbacks.cend(); ++it) {
        if (*it == callback) {
            keyCallbacks.erase(it);
            break;
        }
    }
}

void nafy::EventDispatch::setParent(EventDispatch &parent) {
    if (this->parent != nullptr && toggled) {
        removeFromParent();
    }
    this->parent = &parent;
    if (toggled) {
        addToParent();
    }
}

void nafy::EventDispatch::runToggle() {
    if (toggled) {
        removeFromParent();
    } else {
        addToParent();
    }
}

void nafy::EventDispatch::removeFromParent() {
    parent->removeKeyCallback(this);
    parent->removeMouseButtonCallback(this);
    parent->removeMousePosCallback(this);
}

void nafy::EventDispatch::addToParent() {
    parent->addKeyCallback(*this);
    parent->addMouseButtonCallback(*this);
    parent->addMousePosCallback(*this);
}

void nafy::EventDispatch::setAsRoot(GLFWwindow *window) {
    callbacks[window] = this;
}

void nafy::EventDispatch::unroot() {
    for (cb_location_t it = callbacks.cbegin(); it != callbacks.cend(); ++it) {
        if (it->second == this) {
            it = callbacks.erase(it);
        }
    }
}

bool nafy::EventDispatch::isToggled() {
    return toggled;
}
void nafy::EventDispatch::setToggled(bool value) {
    if (toggled != value) {
        // The value is relevent
        if (parent != nullptr) {
            // Action should be taken
            runToggle();
        }
        toggled = value;
    }
}
