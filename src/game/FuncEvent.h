#ifndef FUNCEVENT_H_INCLUDED
#define FUNCEVENT_H_INCLUDED

#include <functional>

#include "sceneEvent.h"

/*
* A templated typedef'd class that calls a single function.
* Since the class is templated, the user has the choice of
* using a function with no parameters, or a function that
* takes the currently running Context and Scene.
*/

namespace nafy {
    class Context;
    class Scene;

    template<class T>
    class FuncEventBase: public sceneEvent {
    public:
        typedef T callback_t;
    private:
        callback_t callback;
    public:
        FuncEventBase(const callback_t &callback);
        // Change the function
        void set(const callback_t &callback);
        // Executes the callback
        void init(Context *ctx, Scene *parent) override;
        // Always returns true
        bool action(Context *ctx, Scene *parent) override;
    };

    // Template is pre-generated, so please use one of these...
    typedef FuncEventBase<std::function<void(Context*, Scene*)>> FuncEvent;
    typedef FuncEventBase<std::function<void()>> FreeFuncEvent;
}

#endif
