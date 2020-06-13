#include "Scene.h"

#include <memory>

#include "TextString.h"
#include "../core/Context.h"
#include "../core/error.h"

nafy::Scene::Scene(): startIndex(0) {
}

void nafy::Scene::push(BasicEvent::init_t initFunc, BasicEvent::action_t actionFunc) {
    events.push_back(std::make_shared<BasicEvent>(initFunc, actionFunc));
}

void nafy::Scene::pushText(const std::string &str) {
    events.push_back(std::make_shared<TextString>(str));
}

void nafy::Scene::pushText(const std::string &str, unsigned int cooldownMillis) {
    events.push_back(std::make_shared<TextString>(str, cooldownMillis));
}

void nafy::Scene::pushEvent(const sceneEvent_t &e) {
    events.push_back(e);
}

void nafy::Scene::pushFunc(const FuncEvent::callback_t &func) {
    events.push_back(std::make_shared<FuncEvent>(func));
}

void nafy::Scene::pushFreeFunc(const FreeFuncEvent::callback_t &func) {
    events.push_back(std::make_shared<FreeFuncEvent>(func));
}

nafy::Scene &nafy::Scene::operator<<(BasicEvent &&e) {
    events.push_back(std::make_shared<BasicEvent>(std::move(e)));
    return *this;
}

nafy::Scene &nafy::Scene::operator<<(const std::string &str) {
    pushText(str);
    return *this;
}

nafy::Scene &nafy::Scene::operator<<(const sceneEvent_t &e) {
    pushEvent(e);
    return *this;
}

nafy::Scene &nafy::Scene::operator<<(const FuncEvent::callback_t &func) {
    pushFunc(func);
    return *this;
}

nafy::Scene &nafy::Scene::operator<<(const FreeFuncEvent::callback_t &func) {
    pushFreeFunc(func);
    return *this;
}

void nafy::Scene::setStart(events_index_t i) {
    startIndex = i;
}

nafy::Scene::events_index_t nafy::Scene::getStart() {
    return startIndex;
}

nafy::Scene::events_t &nafy::Scene::getEvents() {
    return events;
}


void nafy::Scene::setIndex(events_index_t i) {
    if (events.size()) {
        if (i >= events.size()) {
            index = events.size() - 1;
        } else {
            index = i;
        }
    } else {
        index = 0;
    }
}

void nafy::Scene::setIndexInit(events_index_t i, Context *ctx) {
    setIndex(i);
    if (events.size()) {
        events[index]->init(ctx, this);
    } else {
        throw error("Cannot init index as there are no events");
    }
}

nafy::Scene::events_index_t nafy::Scene::getIndex() {
    return index;
}

void nafy::Scene::init(Context *ctx) {
    if (events.size()) {
        setIndex(startIndex);
        events[index]->init(ctx, this);
    } else {
        throw error("Invalid Scene, must have at least one event");
    }
}

void nafy::Scene::run(Context *ctx) {
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

