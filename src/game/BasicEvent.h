#ifndef BASICEVENT_H_INCLUDED
#define BASICEVENT_H_INCLUDED

#include <functional>

#include "sceneEvent.h"

/*
* As the name suggests, this is the most basic form of an event.
* It takes in two functions, one for init and one for action,
* and runs those when their respective functions are called.
* If you just want to run something once (a one-off event),
* see FuncEvent.
*/

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
        // Change the functions
        void set(init_t initFunc, action_t actionFunc);
        // Executes initFunc
        void init(Context *ctx, Scene *parent) override;
        // Executes actionFunc
        bool action(Context *ctx, Scene *parent) override;
    };
};

#endif
