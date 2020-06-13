#include "FuncEvent.h"

typedef std::function<void()> FreeFuncEvent_t;
typedef std::function<void(nafy::Context*, nafy::Scene*)> funcEvent_t;

template<class T>
nafy::FuncEventBase<T>::FuncEventBase(const callback_t &callback): callback(callback) {
}
template<class T>
void nafy::FuncEventBase<T>::set(const callback_t &callback) {
    this->callback = callback;
}
// Template specialization, depending on the type of T
template<>
void nafy::FuncEventBase<FreeFuncEvent_t>::init(Context *ctx, Scene *parent) {
    callback();
}
template<>
void nafy::FuncEventBase<funcEvent_t>::init(Context *ctx, Scene *parent) {
    callback(ctx, parent);
}
template<class T>
bool nafy::FuncEventBase<T>::action(Context *ctx, Scene *parent) {
    return true;
}

// Generate templates
template class nafy::FuncEventBase<FreeFuncEvent_t>;
template class nafy::FuncEventBase<funcEvent_t>;
