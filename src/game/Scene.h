#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include <vector>
#include <string>

#include "BasicEvent.h"
#include "FuncEvent.h"
#include "sceneEvent.h"

/*
* A scene, contains a sequence of instructions called "events"
* executed by a Context during its game loop.
* You can switch between them and jump to different locations...
* You have to be safe about it though, for every event must have init() called
* once and action() called every frame, and we're dealing with raw pointers...
*/

namespace nafy {
    // Forward decleration to get around circular dependency. Get it?
    class Context;

    class Scene {
    public:
        typedef std::vector<sceneEvent_t> events_t;
        // "an unsigned integral type"
        // http://www.cplusplus.com/reference/vector/vector/
        // Hence, we don't have to do any lower-bounds checks
        typedef events_t::size_type events_index_t;
    private:
        // Don't remove anything if currently running UNLESS you also call init()
        // so that the `index`/current event is set to `startIndex`, or some other
        // legal index. Yes, that means that the Scene must be reset. This is because
        // the index may be offest by the insertion, causing events to be potentually
        // skipped, either in their init() not being called, their action, or both.
        // Although it shouldn't be that big an issue unless you're doing multithreading witchery.
        // Normally, if it's on the same thread, (that is, from an event function called by
        // the Scene), insertions and appendations shouldn't be a problem unless they're before
        // the current index, in which case the previous event will start being called.
        // 
        // Adding things while the Scene is running is perfectly allowed
        events_t events;
        events_index_t index;
        events_index_t startIndex;
        // std::string textBuffer; // For building strings
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
        events_index_t getStart();

        // Not really used anywhere, but you might find a use for it
        events_t &getEvents();

        // If i < events.size(), index = i
        // If i >= events.size(), index = events.size()-1.
        // Does NOT call the init() function of the newly set event
        // (events[index].init()), because it doesn't have any
        // information as to whether it has been called or not,
        // nor does it know what context to give to the event.
        void setIndex(events_index_t i);
        // Calls the above and tries to call events[index].init(ctx, this).
        // If events.size() is 0 however, will not call events[index].init()
        // and instead will throw an instance of `error` (similar to how init(...)
        // will throw an error if the scene's events vector is empty)
        void setIndexInit(events_index_t i, Context *ctx);
        events_index_t getIndex();

        /* private */

        // Called once when set current on a Context
        // Throws instance of `error` if the scene is invalid, ie, no events
        void init(Context *ctx);
        // Called every frame, in turn calls init(...) and action(...) on events
        // until it reaches the end of the vector
        void run(Context *ctx);
    };
};

#endif
