#include "context.h"

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>

#include <cmath> // DEBUG

#include "ShaderFactory.h"
#include "error.h"
#include "env.h"

#include "text/ftype.h"
#include "text/Font.h"
#include "text/Text.h"

void nafy::context::makeCurrent() {
    glfwMakeContextCurrent(window);
    setContext(this);
}

nafy::context::context(int winWidth, int winHeight, const char *winTitle):
    window(plusContext(winWidth, winHeight, winTitle)),
    defaultShaders{
        ShaderFactory("resources/shaders/sprite.vert", "resources/shaders/sprite.frag").make(),
        ShaderFactory("resources/shaders/prim.vert", "resources/shaders/prim.frag").make()
    },
    defaultFont(makeFont(FontFactory(getPath("resources/fonts/Arial.ttf")))),
    root(nullptr), current(nullptr), run(false), runGameAction(false), vsync(0)
    {

    makeCurrent();
    // defaultShaders.sprite = ShaderFactory("resources/shaders/sprite.vert", "resources/shaders/sprite.frag").make();
    // defaultShaders.prim = ShaderFactory("resources/shaders/prim.vert", "resources/shaders/prim.frag").make();

    setFPS(60);

    registerCallbacks(window, this);

    views.reset(new views_s());
    crawl.reset(new TextCrawl());
    if (glGetError() != GL_NO_ERROR) std::cout << "ERROR-321" << std::endl; 

    views_s::home_s &home = views->home; // Shortcut, serves no other purpose

    home.background.setHex(0x2f67f5);
    home.title.setString("NAFY");
    home.title.setX(100);
    home.title.setY(10);
    home.title.generate();
    home.startGame.setX(100);
    home.startGame.setY(50);
    home.startGame.getBox().getColor().setHex(0x1a5d6e);
    home.startGame.getText().setString("Start game!");
    // home.startGame.getText().getAlign();
    std::cout << "eq to le " << (home.startGame.getText().getAlign() == Font::textAlign::center) << std::endl;
    home.startGame.setOnClick([](int button, int mods) -> void {
        std::cout << "Start game~!" << std::endl;
    });
    home.startGame.setOnEnter([&home]() -> void {
        home.startGame.getBox().getColor().setHex(0x3e7887);
    });
    home.startGame.setOnLeave([&home]() -> void {
        home.startGame.getBox().getColor().setHex(0x1a5d6e);
    });
    home.startGame.generate();

    setBackground(home.background);
    setView(home.view);

    home.view.add(&home.title);
    home.view.add(&home.startGame);


    // crawl.setFont(defaultFont);
    // crawl.bindShader(textShader.get());

}

nafy::context::~context() {
    deleteCallbacks(this);
    minusContext(window);
}

void nafy::context::mousePosCallback(double x, double y) {
    for (mouseMoveCallback *&callback : cursorPosCallbacks) {
        callback->mouseMoved(x, y);
    }
}

void nafy::context::mouseButtonCallback(int button, int action, int mods) {
    const bool isPressed = action == GLFW_PRESS;
    for (mouseClickCallback *&callback : cursorButtonCallbacks) {
        callback->mouseClicked(isPressed, button, mods);
    }
}

void nafy::context::addMousePosCallback(mouseMoveCallback &callback) {
    cursorPosCallbacks.push_back(&callback);
}

void nafy::context::addMouseButtonCallback(mouseClickCallback &callback) {
    cursorButtonCallbacks.push_back(&callback);
}

void nafy::context::removeMousePosCallback(mouseMoveCallback *callback) {
    for (std::list<mouseMoveCallback*>::iterator it = cursorPosCallbacks.begin(); it != cursorPosCallbacks.end(); ++it) {
        if (*it == callback) {
            cursorPosCallbacks.erase(it);
            break;
        }
    }
}

void nafy::context::removeMouseButtonCallback(mouseClickCallback *callback) {
    for (std::list<mouseClickCallback*>::iterator it = cursorButtonCallbacks.begin(); it != cursorButtonCallbacks.end(); ++it) {
        if (*it == callback) {
            cursorButtonCallbacks.erase(it);
            break;
        }
    }
}

void nafy::context::activate() {
    makeCurrent();
}

void nafy::context::setRoot(scene &root) {
    this->root = &root;
}

void nafy::context::setCurrent(scene &current) {
    this->current = &current;
    this->current->init(this);
}

void nafy::context::revert() {
    setCurrent(*root);
}

void nafy::context::start() {
    revert();
    resume();
}

void nafy::context::resume() {
    if (current == nullptr) {
        throw error("`current` must NOT be nullptr. Did you forget to call setRoot(scene&)?");
    }
    run = true;

    makeCurrent();

    while (!shouldStop()) {

        const float end = glfwGetTime() + frameCooldown;

        glClearColor(
            background->get()[0],
            background->get()[1],
            background->get()[2],
            background->get()[3]
        );
        glClear(GL_COLOR_BUFFER_BIT);

        if (runGameAction) {
            current->run(this);
        }

        view->render();

        glfwSwapBuffers(window);

        do {
            glfwPollEvents();
            // So that it isn's a total resource hog while waiting
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        } while (!shouldStop() && glfwGetTime() < end);

        // TEMP
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "GL ERROR: " << std::hex << error << std::dec << ": " << getGLErrorStr(error) << std::endl;
            exit(1);
        }
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

void nafy::context::setView(View &view) {
    this->view = &view;
}
void nafy::context::setBackground(Color &color) {
    background = &color;
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

Font::type nafy::context::makeFont(const FontFactory &factory) {
    textLib.makeFont(factory);
    return textLib.makeFont(factory);
}

void nafy::context::setDefaultSpriteShader(const Shader &shader) {
    defaultShaders.sprite = shader;
}
void nafy::context::setDefaultPrimShader(const Shader &shader) {
    defaultShaders.prim = shader;
}
void nafy::context::setDefaultFont(const Font::type &font) {
    defaultFont = font;
}

std::shared_ptr<nafy::context::views_s> nafy::context::getViews() {
    return views;
}

nafy::shader_t nafy::context::getDefaultSpriteShader() {
    return defaultShaders.sprite.get();
}
nafy::shader_t nafy::context::getDefaultPrimShader() {
    return defaultShaders.prim.get();
}
Font::type nafy::context::getDefaultFont() {
    return defaultFont;
}

TextCrawl &nafy::context::getCrawl() {
    return *crawl.get();
}

bool nafy::context::shouldStop() {
    return glfwWindowShouldClose(window) || !run;
}
