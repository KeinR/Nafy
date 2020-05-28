#include "context.h"

#include <iostream>

#include "error.h"
#include "env.h"

#include "shaders.h"
#include "text/ftype.h"
#include "text/Font.h"
#include "text/Text.h"

inline void nafy::context::makeCurrent() {
    glfwMakeContextCurrent(window);
}

nafy::context::context(int winWidth, int winHeight, const char *winTitle, scene &root, Font &initCrawlFont, shader_t crawlShader):
    window(plusContext(winWidth, winHeight, winTitle)), root(&root), current(&root), run(false), textShader(textShader), vsync(0) {
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        throw ft_error("Failed to init freetype library", error);
    }
    crawlFace = initCrawlFont.mkFace(ftLibrary, error);
    if (error) {
        throw ft_error("Failed to make initial crawl face", error);
    }
    crawl.setFace(crawlFace);
    crawl.bindShader(crawlShader);

    setFPS(60);
}

nafy::context::~context() {
    minusContext(window);
}

void nafy::context::setRoot(scene &root) {
    this->root = &root;
}

void nafy::context::setCurrent(scene &current) {
    this->current = &current;
    this->current->reset();
}

void nafy::context::revert() {
    setCurrent(root);
}

void nafy::context::start() {
    revert();
    resume();
}

void nafy::context::resume() {
    if (current == nullptr) {
        throw error("`current` must NOT be null");
    }
    run = true;

    makeCurrent();

    text.generate();


    while (run && engineUp()) {

        const float end = glfwGetTime() + frameCooldown;

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        current->run(this);

        glUseProgram(textShader);
        crawl.render();

        glfwSwapBuffers(window);

        glfwPollEvents();

        while (glfwGetTime() < end);
    }
}

void nafy::context::stop() {
    run = false;
}

void nafy::context::stopIfCurrent(scene *obj) {
    if (obj == current) {
        stop();
    }
}

void nafy::context::setFPS(unsigned int fps) {
    framesPerSecond = fps;
    frameCooldown = 1.0f / fps;
}

unsigned int nafy::context::getFPS() {
    return framesPerSecond;
}

void nafy::context::setVSync(int lv) {
    vsync = lv;
    makeCurrent();
    glfwSwapInterval(vsync);
}

int nafy::context::getVSync() {
    return vsync;
}

Face nafy::context::makeFace(Font &font) {
    FT_Error error;
    Face face = font.mkFace(ftLibrary, error);
    if (error) {
        throw ft_error("Font::mkFace Failed", error);
    }
    return face;
}
