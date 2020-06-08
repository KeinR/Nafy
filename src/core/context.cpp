#include "context.h"

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>

#include <cmath> // DEBUG

#include "../shaders/ShaderFactory.h"
#include "error.h"
#include "../env/env.h"

#include "../text/ftype.h"
#include "../text/Font.h"
#include "../text/Text.h"

#include "../gui/Image.h"

#include "../audio/Device.h"
#include "../audio/AudioContext.h"
#include "../audio/SoundData.h"
#include "../audio/SoundBuffer.h"
#include "../audio/Speaker.h"

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

    setFPS(60);

    registerCallbacks(window, this);

    views.reset(new views_s());

    // Shortcuts, serve no other purpose
    views_s::home_s &home = views->home;
    views_s::game_s &game = views->game;

    // Init home screen

    home.background.setHex(0x2f67f5);
    home.title.setString("NAFY");
    home.title.setX(100);
    home.title.setY(10);
    home.title.generate();
    home.startGame.setX(100);
    home.startGame.setY(50);
    home.startGame.getDisplay().getBox().getColor().setHex(0x1a5d6e);
    home.startGame.getDisplay().getText().setString("Start game!");
    home.startGame.setOnClick([this](int button, int mods) -> void {
        std::cout << "Start game~!" << std::endl;
        this->setView(this->getViewsRef().game.view);
        this->setBackground(this->getViewsRef().game.background);
        this->getViewsRef().home.startGame.setEnabled(false);
        this->setGameRunning(true);
    });
    home.startGame.setOnEnter([&home]() -> void {
        home.startGame.getDisplay().getBox().getColor().setHex(0x3e7887);
    });
    home.startGame.setOnLeave([&home]() -> void {
        home.startGame.getDisplay().getBox().getColor().setHex(0x1a5d6e);
    });
    home.startGame.setCornerRadius(20);
    home.startGame.generate();

    home.view.add(&home.title);
    home.view.add(&home.startGame);

    // Setup game screen

    game.background.setHex(0x00c465);
    game.crawl.setX(10);
    game.crawl.setY(winHeight - 60);
    game.crawl.setHeight(50);
    game.crawl.setWidth(winWidth - 10 * 2);
    game.crawl.getBox().getColor().setHex(0x7d7fff);
    game.crawl.setMargin(5);

    game.view.add(&game.crawl);


    setBackground(home.background);
    setView(home.view);

}

nafy::context::~context() {
    deleteCallbacks(this);
    minusContext(window);
}

void nafy::context::runFrame() {
    if (runGameAction) {
        current->run(this);
    }
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
    for (std::vector<mouseMoveCallback*>::const_iterator it = cursorPosCallbacks.cbegin(); it != cursorPosCallbacks.cend(); ++it) {
        if (*it == callback) {
            cursorPosCallbacks.erase(it);
            break;
        }
    }
}

void nafy::context::removeMouseButtonCallback(mouseClickCallback *callback) {
    for (std::vector<mouseClickCallback*>::const_iterator it = cursorButtonCallbacks.cbegin(); it != cursorButtonCallbacks.cend(); ++it) {
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

    std::cout << "Initializing OpenAL... ";
    Device device;
    AudioContext ctx(device);
    ctx.bind();
    std::cout << "done." << std::endl;
    std::cout << "Loading... ";
    // SoundData testMusic = loadOggVorbisFile("test.ogg");
    std::cout << "done." << std::endl;
    std::cout << "Load ogg" << std::endl;
    SoundBuffer buffer(loadWavFile("test.wav"));
    std::cout << "done" << std::endl;
    Speaker speaker;
    speaker.setGain(1);
    speaker.setPitch(1);
    speaker.setBuffer(buffer);
    // speaker.setTime(120);
    std::cout << "Start play" << std::endl;
    speaker.play();
    std::cout << "done" << std::endl;

    // testMusic.data.reset();


    while (!shouldStop()) {

        const float end = glfwGetTime() + frameCooldown;

        glClearColor(
            background->get()[0],
            background->get()[1],
            background->get()[2],
            background->get()[3]
        );
        glClear(GL_COLOR_BUFFER_BIT);

        view->render();

        glfwSwapBuffers(window);
        
        runFrame();

        do {
            glfwPollEvents();
            // So that it isn's a total resource hog while waiting.
            // For some reason, this works... At least on my machine
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        } while (!shouldStop() && glfwGetTime() < end);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            throw gl_error(error);
        }
    }

}

void nafy::context::stop() {
    run = false;
}

void nafy::context::stopIfCurrent(scene *obj) {
    if (obj == current) {
        setGameRunning(false);
    }
}

void nafy::context::setGameRunning(bool value) {
    runGameAction = value;
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

nafy::context::views_s &nafy::context::getViewsRef() {
    return *views;
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
    return views->game.crawl.getText();
}

bool nafy::context::shouldStop() {
    return glfwWindowShouldClose(window) || !run;
}
