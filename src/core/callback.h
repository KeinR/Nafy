#ifndef CALLBACK_H_INCLUDED
#define CALLBACK_H_INCLUDED

namespace nafy {
    // No virtual deconstructors because these
    // won't be seeing a delete
    class mouseMoveCallback {
    public:
        virtual void mouseMoved(double mouseX, double mouseY) = 0;
    };
    class mouseClickCallback {
    public:
        // Misleading I suppose, because it can be released too...
        virtual void mouseClicked(bool isPressed, int button, int mods) = 0;
    };
    class keyCallback {
    public:
        virtual void keyAction(int key, int scancode, int action, int mods) = 0;
    };
};

#endif
