#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include <vector>
#include <string>

#include "BasicEvent.h"
#include "FuncEvent.h"
#include "sceneEvent.h"

// Think about scenes as single threads of execution,
// where you can switch between them and seek.
// That's how it works with Scenes and their events

namespace nafy {
    // Forward decleration to get around circular dependency
    class Context;

    class Scene {
    public:
        typedef std::vector<sceneEvent_t> events_t;
        typedef events_t::size_type events_index_t;
    private:
        // Don't remove anything if currently running UNLESS you also call init(),
        events_t events;
        events_index_t index;
        events_index_t startIndex;
        std::string textBuffer; // For building
    public:
        Scene();
        // Basic functions
        void push(BasicEvent::init_t initFunc, BasicEvent::action_t actionFunc);
        // Text
        void pushText(const std::string &str);
        void pushText(const std::string &str, unsigned int cooldownMillis);
        // Just any event
        void pushEvent(const sceneEvent_t &e);
        // Emplaces a FuncEvent (context and scene as parameters in callback)
        void pushFunc(const FuncEvent::callback_t &func);
        // Emplaces a FreeFuncEvent (no parameters in callback)
        void pushFreeFunc(const FreeFuncEvent::callback_t &func);

        // For convinience, moves `e` to dynamically allocated BasicEvent
        // Call it like << {x,y}
        Scene &operator<<(BasicEvent &&e);
        // For convinience, calls pushText(const std::string&)
        Scene &operator<<(const std::string &str);
        // For convinience, calls pushEvent(sceneEvent_t)
        Scene &operator<<(const sceneEvent_t &e);
        // For convineince, calls pushFunc()
        Scene &operator<<(const FuncEvent::callback_t &func);
        // For convineince, calls pushFreeFunc()
        Scene &operator<<(const FreeFuncEvent::callback_t &func);

        // Changes the start index to `i`, with the first event being at 0.
        // Clamps to 0 and events.size()-1 in case of invalid bounds
        // This will only have a visible effect the next time the Scene is run from
        // the start, as `index` is only set to `startIndex` when init() is called
        void setStart(events_index_t i);

        /* private */

        // Throws instance of `error` if the scene is invalid, ie, no events
        void init(Context *ctx);
        void run(Context *ctx);
    };
};

#endif
