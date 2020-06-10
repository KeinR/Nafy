#include "BasicEvent.h"

nafy::BasicEvent::BasicEvent(init_t initFunc, action_t actionFunc):
initFunc(initFunc), actionFunc(actionFunc) {
}
void nafy::BasicEvent::init(context *ctx, Scene *parent) {
    initFunc(ctx, parent);
}
bool nafy::BasicEvent::action(context *ctx, Scene *parent) {
    return actionFunc(ctx, parent);
}
