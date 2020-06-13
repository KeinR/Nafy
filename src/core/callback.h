#ifndef CALLBACK_H_INCLUDED
#define CALLBACK_H_INCLUDED

/*
* Callbacks used by Context for different input types.
* Extend one of these to get callback capabilities.
* More lightweight than std::function
*/

namespace nafy {
    // Mouse moved
    class mouseMoveCallback {
    public:
        virtual ~mouseMoveCallback() = 0;
        virtual void mouseMoved(double mouseX, double mouseY) = 0;
    };
    // Mouse button clicked
    class mouseClickCallback {
    public:
        virtual ~mouseClickCallback() = 0;
        // Misleading I suppose, because it can be released too...
        virtual void mouseClicked(bool isPressed, int button, int mods) = 0;
    };
    // Key (on keyboard) pressed/released
    class keyCallback {
    public:
        virtual ~keyCallback() = 0;
        virtual void keyAction(int key, int scancode, int action, int mods) = 0;
    };
};

#endif
