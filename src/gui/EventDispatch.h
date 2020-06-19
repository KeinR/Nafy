#ifndef ACTIVEGROUP_H_INCLUDED
#define ACTIVEGROUP_H_INCLUDED

#include <vector>

#include "../core/callback.h"
#include "../core/glfw.h"
#include "../core/error.h"
#include "renderable.h"

/*
* Handles the dispatching of events.
* Sho
*/

namespace nafy {

    // The global input callbacks.
    // Due to the nature of glfw, it only accepts global function pointers or
    // captureless lambdas... Therefore, all events are processed by these, and each time
    // we must iterate through the `callbacks` vector to find a window and it's associated
    // context to dispatch the events properly
    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    void keyActionCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    class EventDispatch: public mouseMoveCallback, public mouseClickCallback, public keyCallback, public renderable {
    public:
        typedef std::vector<mouseMoveCallback*> move_cont_t;
        typedef std::vector<mouseClickCallback*> click_cont_t;
        typedef std::vector<keyCallback*> key_cont_t;
        typedef std::vector<renderable*> renders_cont_t;
    public: // TEMP
        // If `this` is recieving events from `parent`.
        // Does nothing if a parent is not set.
        bool toggled;
        // The parent, forwards events to `this`
        // A parent is always optional
        EventDispatch *parent;
        move_cont_t move;
        click_cont_t click;
        key_cont_t key;
        renders_cont_t renders;
        // Adds or removes the EventDispatch from `parent`'s monitors
        // depending on the value of `toggled`
        // Note that this shouldn't be called more than once per state of `toggled`,
        // for having the event functions called more than once is not good.
        // Furthermore, the remove* functions only remove the first occurance.
        void runToggle();
        // Removes `this` from `parent`'s event monitors
        // NB: Does NOT check if parent is nullptr!
        void removeFromParent();
        // Adds `this` to `parent`'s event monitors
        // NB: Does NOT check if parent is nullptr!
        void addToParent();
        // Removes `this` as root for all windows it was root for
        void unroot();


        template<typename T>
        void addTo(std::vector<T> &cb, T val);
        template<typename T>
        void removeFrom(std::vector<T> &cb, T val);
        template<typename T, typename F, typename... Args>
        void dispatch(std::vector<T> &cb, F func, Args... args);

        void doSetParent(EventDispatch *parent);

        void copy(const EventDispatch &other);
        void steal(EventDispatch &other);

        EventDispatch(bool toggled, EventDispatch *parent);
    public:
        // Default initialize to false and nullptr
        EventDispatch();
        // Initialize with value
        EventDispatch(EventDispatch &parent);
        EventDispatch(bool toggled);
        EventDispatch(bool toggled, EventDispatch &parent);
        EventDispatch(EventDispatch &&other);
        EventDispatch(const EventDispatch &other);
        ~EventDispatch();
        EventDispatch &operator=(EventDispatch &&other);
        EventDispatch &operator=(const EventDispatch &other);

        // These are the internal callbacks, and SHOULD NOT
        // be called recursively, so as to prevent a deadlock.
        // "recursively", as in, from a callback that one of these called.
        void mouseMoved(double mouseX, double mouseY) override;
        void mouseClicked(bool isPressed, int button, int mods) override;
        void keyAction(int key, int scancode, int action, int mods) override;
        void render() override;

        // Add/remove an event callback.
        void addMousePosCallback(mouseMoveCallback &callback);
        void addMouseButtonCallback(mouseClickCallback &callback);
        void addKeyCallback(keyCallback &callback);
        void addRenderCallback(renderable &callback);
        void removeMousePosCallback(mouseMoveCallback *callback);
        void removeMouseButtonCallback(mouseClickCallback *callback);
        void removeKeyCallback(keyCallback *callback);
        void removeRenderCallback(renderable *callback);

        // Removes all callbacks
        void clearAll();
        // Adds `child` to all dispatches
        void addChild(EventDispatch &child);
        // Removes `child` from all dispatches
        void removeChild(EventDispatch *child);

        move_cont_t &getMoveCallbacks();
        click_cont_t &getClickCallbacks();
        key_cont_t &getKeyCallbacks();
        renders_cont_t &getRenderCallbacks();

        // Set the event dispatch that `this` will take events from
        // note that this is not necessary
        void setParent(EventDispatch &parent);
        // Sets `parent` to nullptr
        void resetParent();
        EventDispatch *getParent();

        // Sets this dispatch as the window's root
        void setAsRoot(GLFWwindow *window);

        // Query state
        bool isToggled();

        // Sets whether the dispatch should recieve events.
        // If `parent` isn't nullptr and `toggle` != value:
        //      if `toggle` is true, removes itself from `parent`'s event dispatch
        //      if `toggle` is false, adds itself to `parent`'s event dispatch
        // if `parent` is nullptr and `value` is true, the EventDispatch
        // will be added as soon as a parent is set through setParent(..)
        void setToggled(bool value);
    };
}

#endif
