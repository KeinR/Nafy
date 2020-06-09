#include "TextString.h"

#include <iostream>

#include "Scene.h"

#include "../core/glfw.h"
#include "../core/context.h"

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

void nafy::TextString::init(context *ctx, Scene *parent) {
    index = 0;
    next = glfwGetTime() + wait;
    ctx->getCrawl().setString(str);
    ctx->getCrawl().generate();
    rolling = true;
}

bool nafy::TextString::action(context *ctx, Scene *parent) {
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
            for (; index < str.length() && isWhitespace(str[index]); index++) {
                if (ctx->getCrawl().advance()) {
                    rolling = false;
                    return false;
                }
            }
            index++;
            if (ctx->getCrawl().advance()) {
                rolling = false;
            }
        }
    }
    return false;
}

