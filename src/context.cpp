#include "context.h"

#include <iostream>

#include "error.h"
#include "env.h"

#include "text/ftype.h"
#include "text/Font.h"
#include "text/Text.h"

void nafy::context::makeCurrent() {
    glfwMakeContextCurrent(window);
}

nafy::context::context(int winWidth, int winHeight, const char *winTitle, scene &root, Font &initCrawlFont, Shader &textShaderB):
    window(plusContext(winWidth, winHeight, winTitle)),
    crawlFace(textLib.makeFace(initCrawlFont)),
    root(&root), current(&root), run(false), vsync(0) {

    makeCurrent();
    textShader = textShaderB.make();
    crawl.setFace(crawlFace);
    crawl.bindShader(textShader.get());

    setFPS(60);
}

nafy::context::~context() {
    minusContext(window);
    std::cout << "Sucs" << std::endl;
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
        throw error("`current` must NOT be null");
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
    crawl.setString("Sure man");
    crawl.generate();

    Shader rs("resources/shaders/prim.vert", "resources/shaders/prim.frag");
    ShaderProgram program = rs.make();
    Rectangle rec(program.get());
    rec.getColor().setHex(0x1f6b33);
    rec.setCornerRadius(5);
    rec.generate();

    while (!shouldStop()) {

        const float end = glfwGetTime() + frameCooldown;

        glClearColor(0.1, 1, 1, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        current->run(this);

        glUseProgram(program.get());
        rec.render();
        glUseProgram(textShader.get());
        crawl.render();
        // std::cout << "ads " << (std::size_t)window << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();

        while (glfwGetTime() < end);
    }

    // {
    //     Font font("resources/fonts/Arial.ttf");
    //     GLFWwindow *window = plusContext(400, 400, "foo");
    //     glfwMakeContextCurrent(window);
    //     std::cout << "eq --> " << (glfwGetCurrentContext() == window) << std::endl;
        
    //     shaders::deInit();
    //     shaders::init();

    //     TextLibrary library;
    //     Face face = library.makeFace(font);
    //     Text tc(face, shaders::text);
    //     tc.setString("Sure man");
    //     tc.generate();

    //     while (!glfwWindowShouldClose(window)) {
    //         glClearColor(0.1, 1, 1, 1.0f);
    //         glClear(GL_COLOR_BUFFER_BIT);

    //         glUseProgram(shaders::text);
    //         // tc.advance();
    //         tc.render();
    //         glfwSwapBuffers(window);
    //         glfwWaitEvents();

    //         // std::cout << glGetError() << "" << std::endl;
    //         // if (glGetError() != GL_NO_ERROR) {
    //         //     std::cout << "GL ERROR------" << std::endl;
    //         // }
    //         const char *msg;
    //         if (glfwGetError(&msg) != GLFW_NO_ERROR) {
    //             std::cout << "GLFW ERROR------: " << msg << std::endl;
    //         }
    //     }
    // }

    // GLenum error = glGetError();
    // switch (error) {
    //     case GL_NO_ERROR: std::cout << "NO problem" << std::endl; break;
    //     case GL_INVALID_ENUM: std::cout << "GL_INVALID_ENUM" << std::endl; break;
    //     case GL_INVALID_OPERATION : std::cout << "GL_INVALID_OPERATION" << std::endl; break;
    //     case GL_INVALID_VALUE : std::cout << "GL_INVALID_VALUE" << std::endl; break;
    //     default:
    //         std::cout << "Unknown: " << std::hex << error << std::dec << std::endl;
    // }

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
    return textLib.makeFace(font);
}

TextCrawl &nafy::context::getCrawl() {
    return crawl;
}

Face &nafy::context::getCrawlFace() {
    return crawlFace;
}

bool nafy::context::shouldStop() {
    return glfwWindowShouldClose(window) || !run;
}
