#include "BasicEvent.h"

nafy::BasicEvent::BasicEvent(init_t initFunc, action_t actionFunc):
initFunc(initFunc), actionFunc(actionFunc) {
}
void nafy::BasicEvent::init(Context *ctx, Scene *parent) {
    initFunc(ctx, parent);
}
bool nafy::BasicEvent::action(Context *ctx, Scene *parent) {
    return actionFunc(ctx, parent);
}
