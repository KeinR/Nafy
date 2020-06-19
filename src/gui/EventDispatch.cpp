#include "EventDispatch.h"

#include <iostream> // DEBUG
#include <thread> // DEBUG
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

nafy::EventDispatch::EventDispatch(): EventDispatch(defToggle, defParent) {
}
nafy::EventDispatch::EventDispatch(EventDispatch &parent): EventDispatch(defToggle, &parent) {
}
nafy::EventDispatch::EventDispatch(bool toggled): EventDispatch(defToggle, defParent) {
}
nafy::EventDispatch::EventDispatch(bool toggled, EventDispatch &parent): EventDispatch(defToggle, &parent) {
}
nafy::EventDispatch::EventDispatch(bool toggled, EventDispatch *parent): toggled(false), parent(parent) {
    setToggled(toggled);
}

nafy::EventDispatch::~EventDispatch() {
    if (parent != nullptr && toggled) {
        removeFromParent();
    }
    unroot();
}

nafy::EventDispatch::EventDispatch(EventDispatch &&other): toggled(false), parent(nullptr) {
    steal(other);
}
nafy::EventDispatch::EventDispatch(const EventDispatch &other): toggled(false), parent(nullptr) {
    copy(other);
}
nafy::EventDispatch &nafy::EventDispatch::operator=(EventDispatch &&other) {
    steal(other);
    return *this;
}
nafy::EventDispatch &nafy::EventDispatch::operator=(const EventDispatch &other) {
    copy(other);
    return *this;
}

void nafy::EventDispatch::copy(const EventDispatch &other) {
    move = other.move;
    click = other.click;
    key = other.key;
    renders = other.renders;

    // I want to fuckingnaef.lng.ljnas.egn .lsgnklggfkln algjklegj ;jg; jaw;etgj ewptgjewtjfewtfjejg jA:js;kgfj as;gj;osgjf'awofk3'0pr3p0tkikkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
    doSetParent(other.parent);
    setToggled(other.toggled);
}
void nafy::EventDispatch::steal(EventDispatch &other) {
    move = std::move(other.move);
    click = std::move(other.click);
    key = std::move(other.key);
    renders = std::move(other.renders);    

    doSetParent(other.parent);
    setToggled(other.toggled);
}

template<typename T>
void nafy::EventDispatch::addTo(std::vector<T> &cb, T val) {
    cb.push_back(val);
}
template<typename T>
void nafy::EventDispatch::removeFrom(std::vector<T> &cb, T val) {
    for (typename std::vector<T>::const_iterator it = cb.cbegin(); it != cb.cend(); ++it) {
        if (*it == val) {
            cb.erase(it);
            break;
        }
    }
}
template<typename T, typename F, typename... Args>
void nafy::EventDispatch::dispatch(std::vector<T> &cb, F func, Args... args) {
    for (T &c : cb) {
        (c->*func)(args...);
    }
}

void nafy::EventDispatch::mouseMoved(double mouseX, double mouseY) {
    dispatch(this->move, mouseMoveCallback::mouseMoved, mouseX, mouseY);
}

void nafy::EventDispatch::mouseClicked(bool isPressed, int button, int mods) {
    dispatch(this->click, mouseClickCallback::mouseClicked, isPressed, button, mods);
}

void nafy::EventDispatch::keyAction(int key, int scancode, int action, int mods) {
    dispatch(this->key, keyCallback::keyAction, key, scancode, action, mods);
}

void nafy::EventDispatch::render() {
    dispatch(this->renders, renderable::render);
}

void nafy::EventDispatch::addMousePosCallback(mouseMoveCallback &callback) {
    addTo(move, &callback);
}

void nafy::EventDispatch::addMouseButtonCallback(mouseClickCallback &callback) {
    addTo(click, &callback);
}

void nafy::EventDispatch::addKeyCallback(keyCallback &callback) {
    addTo(key, &callback);
}

void nafy::EventDispatch::addRenderCallback(renderable &callback) {
    addTo(renders, &callback);
}

void nafy::EventDispatch::removeMousePosCallback(mouseMoveCallback *callback) {
    removeFrom(move, callback);
}

void nafy::EventDispatch::removeMouseButtonCallback(mouseClickCallback *callback) {
    removeFrom(click, callback);
}

void nafy::EventDispatch::removeKeyCallback(keyCallback *callback) {
    removeFrom(key, callback);
}

void nafy::EventDispatch::removeRenderCallback(renderable *callback) {
    removeFrom(renders, callback);
}

void nafy::EventDispatch::clearAll() {
    move.clear();
    click.clear();
    key.clear();
    renders.clear();
}
void nafy::EventDispatch::addChild(EventDispatch &child) {
    addMousePosCallback(child);
    addMouseButtonCallback(child);
    addKeyCallback(child);
    addRenderCallback(child);
}
void nafy::EventDispatch::removeChild(EventDispatch *child) {
    removeMousePosCallback(child);
    removeMouseButtonCallback(child);
    removeKeyCallback(child);
    removeRenderCallback(child);
}

nafy::EventDispatch::move_cont_t &nafy::EventDispatch::getMoveCallbacks() {
    return move;
}
nafy::EventDispatch::click_cont_t &nafy::EventDispatch::getClickCallbacks() {
    return click;
}
nafy::EventDispatch::key_cont_t &nafy::EventDispatch::getKeyCallbacks() {
    return key;
}
nafy::EventDispatch::renders_cont_t &nafy::EventDispatch::getRenderCallbacks() {
    return renders;
}

void nafy::EventDispatch::doSetParent(EventDispatch *parent) {
    if (this->parent != nullptr && toggled) {
        removeFromParent();
    }
    this->parent = parent;
    if (this->parent != nullptr && toggled) {
        addToParent();
    }
}

void nafy::EventDispatch::setParent(EventDispatch &parent) {
    doSetParent(&parent);
}

void nafy::EventDispatch::resetParent() {
    doSetParent(nullptr);
}

nafy::EventDispatch *nafy::EventDispatch::getParent() {
    return parent;
}

void nafy::EventDispatch::runToggle() {
    if (toggled) {
        removeFromParent();
    } else {
        addToParent();
    }
}

void nafy::EventDispatch::removeFromParent() {
    parent->removeChild(this);
}

void nafy::EventDispatch::addToParent() {
    parent->addChild(*this);
}

void nafy::EventDispatch::setAsRoot(GLFWwindow *window) {
    callbacks[window] = this;
}

void nafy::EventDispatch::unroot() {
    for (cb_location_t it = callbacks.cbegin(); it != callbacks.cend();) {
        if (it->second == this) {
            it = callbacks.erase(it);
        } else {
            ++it;
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
