#ifndef SCENEEVENT_H_INCLUDED
#define SCENEEVENT_H_INCLUDED

#include <memory>

/*
* The super class of all events given to a Scene, this class specifies an init(...)
* method that is called as soon as the event is fired, and an action(...) method that
* is called every frame as long as the event desires. This goes without saying, but
* Until the event returns true on a call to action(...), the scene will stay on that
* event and not move on to any suceeding event.
*/

namespace nafy {
    class Context;
    class Scene;

    class sceneEvent {
    public:
        virtual ~sceneEvent() = 0;
        // Called once when the Scene changes its index
        virtual void init(Context *ctx, Scene *parent) = 0;
        // Called every frame
        // Return true if parent should move on to next event (stop)
        // Return false to be called again next frame (continue)
        virtual bool action(Context *ctx, Scene *parent) = 0;
    };

    typedef std::shared_ptr<sceneEvent> sceneEvent_t;
};

#endif
