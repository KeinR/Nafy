#ifndef FUNCEVENT_H_INCLUDED
#define FUNCEVENT_H_INCLUDED

#include <functional>

#include "sceneEvent.h"

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

    typedef FuncEventBase<std::function<void(Context*, Scene*)>> FuncEvent;
    typedef FuncEventBase<std::function<void()>> FreeFuncEvent;
}

#endif
