#ifndef SCENEEVENT_H_INCLUDED
#define SCENEEVENT_H_INCLUDED

#include <memory>

namespace nafy {
    class context;
    class Scene;

    class sceneEvent {
    public:
        virtual ~sceneEvent() = 0;
        virtual void init(context *ctx, Scene *parent) = 0; // Called once
        // Return true if parent should move on to next event
        // Return false to be called again next frame
        virtual bool action(context *ctx, Scene *parent) = 0; // Called every frame
    };

    typedef std::shared_ptr<sceneEvent> sceneEvent_t;
};

#endif
