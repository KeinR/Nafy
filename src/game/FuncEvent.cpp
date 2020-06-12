#include "FuncEvent.h"

template<class T>
nafy::FuncEventBase<T>::FuncEventBase(const callback_t &callback): callback(callback) {
}
template<class T>
void nafy::FuncEventBase<T>::set(const callback_t &callback) {
    this->callback = callback;
}
template<>
void nafy::FuncEventBase<std::function<void()>>::init(Context *ctx, Scene *parent) {
    callback();
}
template<>
void nafy::FuncEventBase<std::function<void(nafy::Context*, nafy::Scene*)>>::init(Context *ctx, Scene *parent) {
    callback(ctx, parent);
}
template<class T>
bool nafy::FuncEventBase<T>::action(Context *ctx, Scene *parent) {
    return true;
}

template class nafy::FuncEventBase<std::function<void()>>;
template class nafy::FuncEventBase<std::function<void(nafy::Context*, nafy::Scene*)>>;
