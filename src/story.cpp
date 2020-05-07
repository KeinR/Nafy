#include "story.h"
#include "context.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "error.h"

nafy::storyEvent::~storyEvent() {
}

nafy::textString::textString(const std::string &str): str(str) {
}

nafy::textString::~textString() {
}

void nafy::textString::reset() {
    ch = str.begin();
}

bool nafy::textString::action(context *ctx) {
    if (ch != str.end()) {
        std::cout << *ch;
        std::cout.flush();
        ch++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return false;
    }
    return true;
}



nafy::scene::scene() {

}

nafy::scene::~scene() {
    for (storyEvent *&e : events) {
        delete e;
    }
}

void nafy::scene::pushText(const std::string &str) {
    events.emplace_back(new textString(str));
}

void nafy::scene::pushEvent(const storyEvent &e) {

}

void nafy::scene::reset() {
    if (events.size()) {
        index = 0;
        events[0]->reset();
    } else {
        throw error("Invalid scene, must have at least one event");
    }
}

void nafy::scene::run(context *ctx) {
    if (events[index]->action(ctx)) {
        index++;
        if (index < events.size()) {
            events[index]->reset();
        } else {
            ctx->stopIfCurrent(this);
        }
    }
}
