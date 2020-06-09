#include "Scene.h"

#include <memory>

#include "TextString.h"
#include "../core/context.h"
#include "../core/error.h"

nafy::Scene::Scene() {
}

void nafy::Scene::pushText(const std::string &str) {
    events.push_back(std::make_shared<TextString>(str));
}

void nafy::Scene::pushText(const std::string &str, unsigned int cooldownMillis) {
    events.push_back(std::make_shared<TextString>(str, cooldownMillis));
}

void nafy::Scene::pushEvent(sceneEvent_t e) {
    events.push_back(e);
}

void nafy::Scene::init(context *ctx) {
    if (events.size()) {
        index = 0;
        events[0]->init(ctx, this);
    } else {
        throw error("Invalid Scene, must have at least one event");
    }
}

void nafy::Scene::run(context *ctx) {
    if (events[index]->action(ctx, this)) {
        index++;
        if (index < events.size()) {
            events[index]->init(ctx, this);
        } else {
            // So that is, stop if the line hasn't been set to another Scene,
            // which would indicate that the last event intends for this
            // line to end...
            ctx->stopIfCurrent(this);
        }
    }
}

