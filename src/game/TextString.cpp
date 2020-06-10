#include "TextString.h"

#include <iostream>

#include "Scene.h"

#include "../core/glfw.h"
#include "../core/Context.h"

unsigned int nafy::TextString::defaultCooldown = 50;

static bool isWhitespace(char c) {
    switch (c) {
        case ' ':
        case '\n':
        case '\r':
        case '\t': // ?
            return true;
        default:
            return false;
    }
}

nafy::TextString::TextString(const std::string &str, unsigned int cooldownMillis):
    str(str), wait(0.001f * cooldownMillis), rolling(false) {
}

void nafy::TextString::setString(const std::string &str) {
    this->str = str;
}
void nafy::TextString::setCooldown(unsigned int cooldownMillis) {
    wait = 0.001f * cooldownMillis;
}
std::string nafy::TextString::getString() {
    return str;
}
unsigned int nafy::TextString::getCooldown() {
    return wait / 0.001f;
}

void nafy::TextString::init(Context *ctx, Scene *parent) {
    index = 0;
    next = glfwGetTime() + wait;
    ctx->getCrawl().setString(str);
    ctx->getCrawl().generate();
    rolling = true;
}

bool nafy::TextString::action(Context *ctx, Scene *parent) {
    if (ctx->getUserAdvance()) {
        ctx->setUserAdvance(false);
        if (!rolling) {
            ctx->getCrawl().setString("");
            ctx->getCrawl().generate();
            return true;
        }
        ctx->getCrawl().advance(str.length());
    } else if (rolling) {
        if (glfwGetTime() >= next) {
            next = glfwGetTime() + wait;
            // Failsafe in case string is changed mid-way
            if (index < str.length()) {
                for (; index < str.length() && isWhitespace(str[index]); index++) {
                    if (ctx->getCrawl().advance()) {
                        rolling = false;
                        return false;
                    }
                }
                // dot, dot, dot...
                // Wait longer on ellipsises
                if (str[index] == '.') {
                    next += wait * 5;
                }
                index++;
            }
            if (ctx->getCrawl().advance()) {
                rolling = false;
            }
        }
    }
    return false;
}

