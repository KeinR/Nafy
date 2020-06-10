#ifndef BASICEVENT_H_INCLUDED
#define BASICEVENT_H_INCLUDED

#include <functional>

#include "sceneEvent.h"

namespace nafy {
    class context;
    class Scene;

    class BasicEvent: public sceneEvent {
    public:
        typedef std::function<void(context*, Scene*)> init_t;
        typedef std::function<bool(context*, Scene*)> action_t;
    private:
        init_t initFunc;
        action_t actionFunc;
    public:
        BasicEvent(init_t initFunc, action_t actionFunc);
        void init(context *ctx, Scene *parent) override;
        bool action(context *ctx, Scene *parent) override;
    };
};

#endif
