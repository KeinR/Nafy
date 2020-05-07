#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <string>

#include "glfw.h"

#include "story.h"

namespace nafy {
    class context {
        scene *root;
        scene *current;
        bool run;
    public:
        // Note: root must NOT be nullptr
        context(scene *root);
        void setRoot(scene *root);
        // Sets `this->current` to `current` and resets it
        void setCurrent(scene *current);
        // Sets `current` to `root`
        void revert();
        // calls revert() and resume()
        void start();
        // Start from `current`
        void resume();
        void stop();
        void stopIfCurrent(scene *obj);
    };
}

#endif
