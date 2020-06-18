#ifndef ACTIVEGROUP_H_INCLUDED
#define ACTIVEGROUP_H_INCLUDED

#include <vector>

#include "../core/callback.h"
#include "../core/glfw.h"

/*
* Handles the dispatching of events
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

    class EventDispatch: public mouseMoveCallback, public mouseClickCallback, public keyCallback {
        bool toggled;
        EventDispatch *parent;
        std::vector<mouseMoveCallback*> moveCallbacks;
        std::vector<mouseClickCallback*> clickCallbacks;
        std::vector<keyCallback*> keyCallbacks;
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
    public:
        // Default initialize to false and nullptr
        EventDispatch();
        // Initialize with value
        EventDispatch(EventDispatch &parent);
        EventDispatch(bool toggled);
        EventDispatch(bool toggled, EventDispatch &parent);
        ~EventDispatch();

        void mouseMoved(double mouseX, double mouseY) override;
        void mouseClicked(bool isPressed, int button, int mods) override;
        void keyAction(int key, int scancode, int action, int mods) override;

        void addMousePosCallback(mouseMoveCallback &callback);
        void addMouseButtonCallback(mouseClickCallback &callback);
        void addKeyCallback(keyCallback &callback);
        void removeMousePosCallback(mouseMoveCallback *callback);
        void removeMouseButtonCallback(mouseClickCallback *callback);
        void removeKeyCallback(keyCallback *callback);

        // Set the event dispatch that `this` will take events from
        // note that this is not necessary
        void setParent(EventDispatch &parent);

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
