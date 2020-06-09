#include "src/game/Scene.h"
#include "src/core/context.h"
#include "src/env/env.h"
#include "src/core/error.h"
#include "src/shaders/ShaderFactory.h"
#include "src/gui/Image.h"

#include <iostream>

using namespace nafy;

int main() {
    try {
        context ctx(600, 400, "test");
        Scene sc;
        sc.pushText("Hello gamers~");
        ctx.setRoot(sc);

        Image img;
        img.loadImage("img.jpg");
        img.setX(0);
        img.setY(0);
        img.setWidth(600);
        img.setHeight(400);
        View &game = ctx.getViewsRef().game.view;
        game.addAt(&img, 0);

        ctx.setSpeaker("Youmu");

        ctx.start();
    } catch (std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    // Font font("resources/fonts/Arial.ttf");
    // {
    //     GLFWwindow *window = nafy::plusContext(400, 400, "foo");
    //     nafy::TextLibrary library;
    //     Face face = library.makeFace(font);
    //     TextCrawl tc(face, nafy::shaders::text);
    //     tc.setString("Sure man");
    //     tc.generate();
    //     while (!glfwWindowShouldClose(window)) {
    //         glClearColor(1, 1, 1, 1.0f);
    //         glClear(GL_COLOR_BUFFER_BIT);

    //         tc.advance();
    //         tc.render();
    //         // glfwSwapBuffers(window);
    //         glfwWaitEvents();
    //     }
    // }

    
    // {
    //     nafy::scene sc;
    //     nafy::context ctx(400, 400, "test", sc, font);
    // }

    // nafy::minusContext(window);

}
