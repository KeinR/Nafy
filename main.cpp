#include "src/game/Scene.h"
#include "src/game/ButtonPrompt.h"
#include "src/game/BasicEvent.h"
#include "src/core/Context.h"
#include "src/env/env.h"
#include "src/core/error.h"
#include "src/shaders/ShaderFactory.h"
#include "src/gui/Image.h"

#include <iostream>

using namespace nafy;

int main() {
    try {
        Context ctx(600, 400, "test");
        ctx.activate();

        Scene sc;
        ctx.setRoot(sc);
        sc <<
        "Hello\2 ga\nmers~" <<
        "I like cheese" <<
        "Did you know that?" <<
        BasicEvent(
            [](Context *ctx, Scene *sc)->void{
                std::cout << "He he init" << std::endl;
            },
            [](Context *ctx, Scene *sc)->bool{
                std::cout << "Wait I'm last!?!?" << std::endl;
                return true;
            }
        ) <<
        "Did you enjoy this? Please answer honestly...";

        std::cout << "GL_NO_ERROR = " << GL_NO_ERROR << std::endl;

        std::shared_ptr<ButtonPrompt> prompt = std::make_shared<ButtonPrompt>();
        prompt->add("Yes", [&ctx]()->void{
            std::cout << ">>> That's pretty cool" << std::endl;
            // ctx.setSpeaker("");
        });
        prompt->add("No", [&ctx]()->void{
            std::cout << ">>> Aw, well, that stucks..." << std::endl;
            ctx.stop();
        });
        prompt->generate();

        sc << prompt;

        Image img;
        img.loadImage("img.jpg");
        img.setX(0);
        img.setY(0);
        img.setWidth(600);
        img.setHeight(400);
        View &game = ctx.getViewsRef().game.view;
        game.addAt(&img, 0);

        Image *img2 = new Image();
        delete img2;

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
    //     nafy::Context ctx(400, 400, "test", sc, font);
    // }

    // nafy::minusContext(window);

}
