#include "story.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "../core/context.h"
#include "../core/glfw.h"
#include "../core/error.h"

nafy::storyEvent::~storyEvent() {
}

nafy::textString::textString(const std::string &str, unsigned int cooldownMillis):
    str(str), wait(0.001f * cooldownMillis) {
}

nafy::textString::~textString() {
}

void nafy::textString::init(context *ctx, scene *parent) {
    next = glfwGetTime() + wait;
    ctx->getCrawl().setString(str);
    ctx->getCrawl().generate();
}

bool nafy::textString::action(context *ctx, scene *parent) {
    if (glfwGetTime() >= next) {
        next = glfwGetTime() + wait;
        return ctx->getCrawl().advance();
    }
    return false;
}



nafy::scene::scene() {
}

nafy::scene::~scene() {
    for (event &e : events) {
        if (e.managed) {
            delete e.ptr;
        }
    }
}

void nafy::scene::pushText(const std::string &str, unsigned int cooldownMillis) {
    events.push_back({new textString(str, cooldownMillis), true});
}

void nafy::scene::pushEvent(storyEvent &e) {
    events.push_back({&e, false});
}

void nafy::scene::init(context *ctx) {
    if (events.size()) {
        index = 0;
        events[0].ptr->init(ctx, this);
    } else {
        throw error("Invalid scene, must have at least one event");
    }
}

void nafy::scene::run(context *ctx) {
    if (events[index].ptr->action(ctx, this)) {
        index++;
        if (index < events.size()) {
            events[index].ptr->init(ctx, this);
        } else {
            // So that is, stop if the line hasn't been set to another scene
            ctx->stopIfCurrent(this);
        }
    }
}
