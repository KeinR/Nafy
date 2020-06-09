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
    str(str), wait(0.001f * cooldownMillis) {
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
}

bool nafy::TextString::action(context *ctx, Scene *parent) {
    if (glfwGetTime() >= next) {
        next = glfwGetTime() + wait;
        for (; isWhitespace(str[index]); index++) {
            if (ctx->getCrawl().advance()) {
                return true;
            }
        }
        index++;
        return ctx->getCrawl().advance();
    }
    return false;
}

