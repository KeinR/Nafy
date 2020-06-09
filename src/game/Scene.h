#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include <vector>
#include <string>

#include "sceneEvent.h"

// Think about scenes as single threads of execution,
// where you can switch between them and seek.
// That's how it works with Scenes and their events

namespace nafy {
    // Forward decleration to get around circular dependency
    class context;

    class Scene {
        // Don't remove anything if currently running UNLESS you also call init(),
        std::vector<sceneEvent_t> events;
        std::vector<sceneEvent_t>::size_type index;
    public:
        Scene();
        void pushText(const std::string &str);
        void pushText(const std::string &str, unsigned int cooldownMillis);
        void pushEvent(sceneEvent_t e);

        /* private */

        // Throws instance of `error` if the scene is invalid, ie, no events
        void init(context *ctx);
        void run(context *ctx);
    };
};

#endif
