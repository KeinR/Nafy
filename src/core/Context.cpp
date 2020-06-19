#include "Context.h"

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

void nafy::Context::makeCurrent() {
    glfwMakeContextCurrent(window);
    setContext(this);
}

nafy::Context::Context(int winWidth, int winHeight, const char *winTitle):
    window(plusContext(winWidth, winHeight, winTitle)),
    defaultShaders{
        ShaderFactory("resources/shaders/sprite.vert", "resources/shaders/sprite.frag").make(
            Shader::uni::sampler0 | Shader::uni::model
        ),
        ShaderFactory("resources/shaders/prim.vert", "resources/shaders/prim.frag").make(
            Shader::uni::color | Shader::uni::model
        )
    },
    defaultFont(makeFont(FontFactory(getPath("resources/fonts/Arial.ttf")))),
    root(nullptr), current(nullptr), run(false), runGameAction(false), userAdvance(false), vsync(0)
{

    makeCurrent();

    setFPS(60);

    dispatch.setAsRoot(window);

    views.reset(new views_s());

    // Shortcuts, serve no other purpose
    views_s::home_s &home = views->home;
    views_s::game_s &game = views->game;
    views_s::menu_s &menu = views->menu;

    // Init home screen

    home.background.setHex(0x2f67f5);
    // The title, centered at the top
    home.title.setString("NAFY");
    home.title.setY(10);
    home.title.setAlign(Font::textAlign::center);
    home.title.generate();
    home.title.setX((winWidth - home.title.getWidth()) / 2);
    // Button that starts the game
    home.startGame.setY(50);
    home.startGame.getColor().setHex(0x1a5d6e);
    home.startGame.getText().setAlign(Font::textAlign::center);
    home.startGame.getText().setString("Start game!");
    home.startGame.setWidth(100);
    home.startGame.setX((winWidth - home.startGame.getWidth()) / 2);
    home.startGame.setOnClick([this](Button *caller, int button, int mods) -> void {
        // Change view to game view
        this->setDispatch(this->getViewsRef().game.dispatch);
        this->setBackground(this->getViewsRef().game.background);
        // Disable button so that it can't be pressed despite being invisable
        this->setGameRunning(true);
        // set cursor to normal (TODO: This is a bad way of doing this!)
        releaseCursor();
    });
    home.startGame.setOnEnter([](Button *caller) -> void {
        setCursorHand();
        caller->getColor().setHex(0x3e7887);
    });
    home.startGame.setOnLeave([](Button *caller) -> void {
        releaseCursor();
        caller->getColor().setHex(0x1a5d6e);
    });
    home.startGame.setCornerRadius(10);
    home.startGame.generate();

    home.startGame.setDispatch(home.dispatch);
    home.startGame.setEnabled(true);
    home.dispatch.addRenderCallback(home.title);
    home.dispatch.addRenderCallback(home.startGame);

    // Setup game screen

    game.background.setHex(0x00c465);
    // The text box with the crawling text, the main focus of any visual novel
    game.crawl.setX(10);
    game.crawl.setY(winHeight - 60);
    game.crawl.setHeight(50);
    game.crawl.setWidth(winWidth - 10 * 2);
    game.crawl.getColor().setHex(0x7d7fff);
    game.crawl.setMargin(5);
    // Top-left corner is pointy because we have the "speaker box" on top of it
    game.crawl.setCornerRadius(0, 0);
    game.crawl.setCornerRadius(1, 7);
    game.crawl.setCornerRadius(2, 7);
    game.crawl.setCornerRadius(3, 7);
    // We're actually using a button so that we can get user input,
    // and the default button text align is center
    game.crawl.getText().setAlign(Font::textAlign::left);
    game.crawl.setOnRelease([this](CrawlButton *caller, int button, int mods) -> void {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            this->setUserAdvance(true);
        }
    });
    game.crawl.generate();
    // The aforementioned "speaker box", displays the name of
    // whoever's currently speaking
    game.speaker.setX(10); // Aligned with the crawl box
    game.speaker.setY(winHeight - 75);
    game.speaker.setHeight(15);
    game.speaker.setWidth(80);
    game.speaker.getBox().getColor().setHex(0xa6a7ff);
    game.speaker.setMargin(2);
    // Smaller font size so that it doesn't stand out as much
    game.speaker.getText().setFontSize(12);
    // Default align for TextRec is left
    game.speaker.getText().setAlign(Font::textAlign::center);
    game.speaker.generate();

    game.crawl.setDispatch(game.dispatch);
    game.dispatch.addRenderCallback(game.crawl);
    game.dispatch.addRenderCallback(game.speaker);

    // Init the "dropdown" menu, this is shown when the user presses escape.
    // It acts as a nexus of sorts that allows the user to go to the main menu,
    // return to game, save, load, quit, etc.
    // TODO: Finish this

    menu.bg.getColor().setVal(0, 0, 0, 0.5);
    menu.topTitle.setString("PAUSED");
    menu.topTitle.generate();
    menu.topTitle.setX((winWidth - menu.topTitle.getWidth()) / 2);
    menu.topTitle.setY(20);
    #define MARGIN 6
    menu.resumeGame.getText().setString("Resume");
    menu.resumeGame.getText().generate();
    menu.resumeGame.setX((winWidth - menu.resumeGame.getText().getWidth() - MARGIN) / 2);
    menu.resumeGame.setY(50);
    menu.resumeGame.setMargin(MARGIN);
    menu.toMenu.getText().setString("Main menu");
    menu.toMenu.getText().generate();
    menu.toMenu.setX((winWidth - menu.toMenu.getText().getWidth() - MARGIN) / 2);
    menu.toMenu.setY(70);
    menu.toMenu.setMargin(MARGIN);
    #undef MARGIN

    menu.resumeGame.setDispatch(menu.dispatch);
    menu.toMenu.setDispatch(menu.dispatch);
    menu.dispatch.addRenderCallback(menu.topTitle);
    menu.dispatch.addRenderCallback(menu.resumeGame);
    menu.dispatch.addRenderCallback(menu.toMenu);

    // Start off at home
    setBackground(home.background);
    setDispatch(home.dispatch);

}

nafy::Context::~Context() {
    minusContext(window);
}

void nafy::Context::runFrame() {
    if (runGameAction) {
        current->run(this);
    }
}

nafy::EventDispatch &nafy::Context::getDispatch() {
    return dispatch;
}

void nafy::Context::activate() {
    // I mean, it's good to have control over what happens when the user calls it...
    makeCurrent();
}

void nafy::Context::setRoot(Scene &root) {
    this->root = &root;
}

void nafy::Context::setCurrent(Scene &current) {
    this->current = &current;
    // init must always be called before run
    this->current->init(this);
}

void nafy::Context::revert() {
    setCurrent(*root);
}

void nafy::Context::start() {
    revert();
    resume();
}

void nafy::Context::resume() {
    if (current == nullptr) {
        throw error("`current` must NOT be nullptr. Did you forget to call setRoot(Scene&)?");
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

        dispatch.render();

        glfwSwapBuffers(window);
        
        runFrame();

        // Check for errors, and throw an exception if there is one.
        // TODO: Add error hooks so that every error doesn't cause a crash

        GLenum glerror = glGetError();
        if (glerror != GL_NO_ERROR) {
            throw gl_error(glerror);
        }

        ALenum alerror = alGetError();
        if (alerror != AL_NO_ERROR) {
            throw al_error(alerror);
        }

        do {
            // Minimal input lag -dabs-
            glfwPollEvents();
            // So that it isn's a total resource hog while waiting.
            // For some reason, this works... At least on my machine
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        } while (!shouldStop() && glfwGetTime() < end);
    }

}

void nafy::Context::stop() {
    run = false;
}

void nafy::Context::stopIfCurrent(Scene *obj) {
    if (obj == current) {
        setGameRunning(false);
    }
}

void nafy::Context::setGameRunning(bool value) {
    runGameAction = value;
}

void nafy::Context::setDispatch(EventDispatch &dis) {
    dispatch.clearAll();
    dispatch.addChild(dis);
    dis.setToggled(true);
    dispatch.setToggled(true);
}
void nafy::Context::setBackground(Color &color) {
    background = &color;
}

void nafy::Context::setFPS(unsigned int fps) {
    framesPerSecond = fps;
    // glfwGetTime() returns seconds as a float
    frameCooldown = 1.0f / fps;
}

unsigned int nafy::Context::getFPS() {
    return framesPerSecond;
}

void nafy::Context::setVSync(int lv) {
    vsync = lv;
    makeCurrent();
    glfwSwapInterval(vsync);
}

int nafy::Context::getVSync() {
    return vsync;
}

Font::type nafy::Context::makeFont(const FontFactory &factory) {
    textLib.makeFont(factory);
    return textLib.makeFont(factory);
}

void nafy::Context::setDefaultSpriteShader(const shader_t &shader) {
    defaultShaders.sprite = shader;
}
void nafy::Context::setDefaultPrimShader(const shader_t &shader) {
    defaultShaders.prim = shader;
}
void nafy::Context::setDefaultFont(const Font::type &font) {
    defaultFont = font;
}

std::shared_ptr<nafy::Context::views_s> nafy::Context::getViews() {
    return views;
}

nafy::Context::views_s &nafy::Context::getViewsRef() {
    return *views;
}

void nafy::Context::setSpeaker(const std::string &name) {
    views->game.speaker.getText().setString(name);
    views->game.speaker.getText().generate();
}

nafy::shader_t nafy::Context::getDefaultSpriteShader() {
    return defaultShaders.sprite;
}
nafy::shader_t nafy::Context::getDefaultPrimShader() {
    return defaultShaders.prim;
}
Font::type nafy::Context::getDefaultFont() {
    return defaultFont;
}

TextCrawl &nafy::Context::getCrawl() {
    return views->game.crawl.getDisplay().getText();
}

nafy::EventDispatch &nafy::Context::getGameDispatch() {
    return views->game.dispatch;
}

bool nafy::Context::shouldStop() {
    return glfwWindowShouldClose(window) || !run;
}

void nafy::Context::setUserAdvance(bool value) {
    userAdvance = value;
}
bool nafy::Context::getUserAdvance() {
    return userAdvance;
}
