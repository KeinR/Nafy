#include "src/audio/AudioContext.h"
#include "src/audio/Device.h"
#include "src/audio/SoundBuffer.h"
#include "src/audio/SoundData.h"
#include "src/audio/Speaker.h"
#include "src/audio/AudioStream.h"
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

        // Device device;
        // AudioContext ac(device);
        // ac.bind();
        // // WavStream stream("test.wav");
        // VorbisStream stream("test.ogg");
        // Speaker speaker;
        // speaker.setGain(1);
        // speaker.setPitch(1);
        // speaker.setBuffer(buffer);

        Scene sc;
        ctx.setRoot(sc);
        sc <<
        "Hello\2 ga\nmers~" <<
        "I like cheese and I relaly don't like to go outside.    Have you tried that? It's                horrible out there!" <<
        []() -> void {
            std::cout << "go play" << std::endl;
            // speaker.playStream(stream);
            std::cout << "ah yes" << std::endl;
        } <<
        "Did you know that?" <<
        BasicEvent(
            [](Context *ctx, Scene *sc)->void{
                // speaker.setTime(0);
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
        EventDispatch::renders_cont_t &game = ctx.getViewsRef().game.dispatch.getRenderCallbacks();
        game.insert(game.begin(), &img);

        Image *img2 = new Image();
        delete img2;

        ctx.setSpeaker("Youmu");

        try {
            ctx.start();
        } catch (std::exception &e) {
            std::cerr << "ERROR IN START: " << e.what() << std::endl;
        }
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
