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

    std::cout << "Lone 1" << std::endl;

    makeCurrent();
    // defaultShaders.sprite = ShaderFactory("resources/shaders/sprite.vert", "resources/shaders/sprite.frag").make();
    // defaultShaders.prim = ShaderFactory("resources/shaders/prim.vert", "resources/shaders/prim.frag").make();

    setFPS(60);

    registerCallbacks(window, this);

    std::cout << "start construct" << std::endl;

    views.reset(new views_s());
    crawl.reset(new TextCrawl());
    if (glGetError() != GL_NO_ERROR) std::cout << "ERROR-321" << std::endl; 

    views_s::home_s &home = views->home; // Shortcut, serves no other purpose

    home.background.setHex(0x2f67f5);
    home.title.setString("NAFY");
    home.title.setX(100);
    home.title.setY(10);
    home.startGame.getBox().getColor().setHex(0x1a5d6e);
    home.startGame.setOnClick([](int button, int mods) -> void {
        std::cout << "Start game~!" << std::endl;
    });
    home.startGame.setOnEnter([&home]() -> void {
        home.startGame.getBox().getColor().setHex(0x3e7887);
    });
    home.startGame.setOnLeave([&home]() -> void {
        home.startGame.getBox().getColor().setHex(0x1a5d6e);
    });

    setBackground(home.background);
    setView(home.view);


    // crawl.setFont(defaultFont);
    // crawl.bindShader(textShader.get());

    std::cout << "done construct" << std::endl;

}

nafy::context::~context() {
    deleteCallbacks(this);
    minusContext(window);
    std::cout << "Sucs" << std::endl;
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
    std::cout << "do revert" << std::endl;
    revert();
    std::cout << "done" << std::endl;
    resume();
}

// #include <stb/stb_image_write.h>

void nafy::context::resume() {
    if (current == nullptr) {
        throw error("`current` must NOT be nullptr. Did you forget to call setRoot(scene&)?");
    }
    std::cout << "tet" << std::endl;
    run = true;

    makeCurrent();
    
    // shaders::deInit();
    // shaders::init();
    
    // GLFWwindow *win = nafy::plusContext(400, 400, "foo");
    // glfwMakeContextCurrent(window);

    // TextCrawl tc(crawlFace, textShader);

    // tc.setString("I like you");
    // tc.generate();
    // tc.advance();
    // stbi_write_png("dasds.png", tc.renderedWidth, tc.renderedHeight, 4, tc.bitmap, tc.renderedWidth * 4);
    // crawl.advance();
    // crawl.advance();
    // crawl.bindShader(shaders::text);
    // crawl.setString("Sure man");
    // crawl.generate();

    // if (glGetError() != GL_NO_ERROR) std::cout << "ERROR-0" << std::endl; 
    // ShaderFactory rs("resources/shaders/prim.vert", "resources/shaders/prim.frag");
    // Shader program = rs.make();
    // Rectangle rec(program.get());
    // rec.getColor().setHex(0x1f6b33);
    // rec.setCornerRadius(30);
    // rec.setX(300);
    // rec.generate();

    // std::cout << "lMake bustton" << std::endl;
    // Button button(crawlFace, textShader.get(), program.get());
    // button.setMargin(0);
    // button.setX(200);
    // button.getBox().getColor().setHex(0x4287f5);
    // button.getText().setString("0x4287f5");
    // button.setOnClick([](int button, int mods)-> void {
    //     std::cout << "YOOOOOOOOOO" << std::endl;
    // });
    // button.setOnEnter([&button]()-> void {
    //     button.getBox().getColor().setHex(0x6d9eed);
    // });
    // button.setOnLeave([&button]()-> void {
    //     button.getBox().getColor().setHex(0x4287f5);
    // });
    // button.generate();

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

        // rec.render();
        // crawl.render();
        // // std::cout << "ads " << (std::size_t)window << std::endl;

        // button.render();

        glfwSwapBuffers(window);

        do {
            glfwPollEvents();
            // So that it isn's a total resource hog while waiting
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        } while (!shouldStop() && glfwGetTime() < end);
        // std::cout << "OVERFLOW: " << std::round((glfwGetTime() - end) * 1000) << " ms" << std::endl;
        // std::cout << "SLEEP TIME: " << std::round(frameCooldown * 1000) << " ms" << std::endl;

        // TEMP
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "GL ERROR: " << std::hex << error << std::dec << ": " << getGLErrorStr(error) << std::endl;
            exit(1);
        }
    }

    std::cout << "exit" << std::endl;
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
    std::cout << "MAKE FINC" << std::endl;
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
    std::cout << "get default sprite" << std::endl;
    return defaultShaders.sprite.get();
}
nafy::shader_t nafy::context::getDefaultPrimShader() {
    std::cout << "get default primitive" << std::endl;
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
