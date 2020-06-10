#ifndef BASICEVENT_H_INCLUDED
#define BASICEVENT_H_INCLUDED

#include <functional>

#include "sceneEvent.h"

namespace nafy {
    class Context;
    class Scene;

    class BasicEvent: public sceneEvent {
    public:
        typedef std::function<void(Context*, Scene*)> init_t;
        typedef std::function<bool(Context*, Scene*)> action_t;
    private:
        init_t initFunc;
        action_t actionFunc;
    public:
        BasicEvent(init_t initFunc, action_t actionFunc);
        void init(Context *ctx, Scene *parent) override;
        bool action(Context *ctx, Scene *parent) override;
    };
};

#endif
