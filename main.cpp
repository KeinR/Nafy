#include "src/audio/AudioContext.h"
#include "src/audio/Device.h"
#include "src/audio/SoundBuffer.h"
#include "src/audio/SoundData.h"
#include "src/audio/Speaker.h"
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

        Device device;
        AudioContext audioContext(device);
        audioContext.bind();
        SoundBuffer sfx_failed(loadWavFile("resources/audio/failed.wav"));
        SoundBuffer sfx_wow(loadWavFile("resources/audio/wow.wav"));
        SoundBuffer sfx_victory(loadWavFile("resources/audio/victory.wav"));

        Speaker speaker;

        Scene sc;
        Scene after;
        Scene branch0;
        branch0 << "He, thanks!" << BasicEvent(
            [&speaker, &sfx_victory, &after](Context *ctx, Scene *scn)->void{
                speaker.setBuffer(sfx_victory);
                speaker.play();
                ctx->setCurrent(after);
            }, [](Context *ctx, Scene *sc)->bool{return true;}
        );
        Scene branch1;
        branch1 << "I guess we all have our opinions..." << BasicEvent(
            [&after](Context *ctx, Scene *scn)->void{
                ctx->setCurrent(after);
            }, [](Context *ctx, Scene *sc)->bool{return true;}
        );
        Scene branch2;
        branch2 << "Wow ok then.." << BasicEvent(
            [&speaker, &sfx_wow, &after](Context *ctx, Scene *scn)->void{
                speaker.setBuffer(sfx_wow);
                speaker.play();
                ctx->setCurrent(after);
            }, [](Context *ctx, Scene *sc)->bool{return true;}
        );

        ctx.setRoot(sc);
        sc <<
        "Hey" <<
        "This is my API thing" <<
        "It's over 6000 lines of C++ code" <<
        BasicEvent(
            [&speaker, &sfx_wow](Context *ctx, Scene *sc)->void{
                speaker.setBuffer(sfx_wow);
                speaker.play();
            }, [](Context *ctx, Scene *sc)->bool{return true;}
        ) <<
        "(...Yes, I have a thing that counts that kind of stuff)" <<
        "And I'm tired as hell" <<
        "There's so much left to do for it..." <<
        "If I could, I'd work for two more weeks" <<
        "However unfortunately, it's due today... In 2 hours." <<
        BasicEvent(
            [&speaker, &sfx_failed](Context *ctx, Scene *sc)->void{
                speaker.setBuffer(sfx_failed);
                speaker.play();
            }, [](Context *ctx, Scene *sc)->bool{return true;}
        ) <<
        BasicEvent(
            [](Context *ctx, Scene *sc)->void{
                ctx->setSpeaker("You");
            }, [](Context *ctx, Scene *sc)->bool{return true;}
        ) <<
        "It looks kinda' scuffed ngl" <<
        BasicEvent(
            [](Context *ctx, Scene *sc)->void{
                ctx->setSpeaker("Orion");
            }, [](Context *ctx, Scene *sc)->bool{return true;}
        ) <<
        "This is just a demonstration. That I threw together at the last minute. There are many features that you don't see..."
         <<
        "What do you think? Please answer honestly...";

        std::shared_ptr<ButtonPrompt> prompt = std::make_shared<ButtonPrompt>();
        prompt->add("Perfect", [&ctx, &branch0]()->void{
            ctx.setCurrent(branch0);
        });
        prompt->add("Meh", [&ctx, &branch1]()->void{
            ctx.setCurrent(branch1);
        });
        prompt->add("Stop trying", [&ctx, &branch2]()->void{
            ctx.setCurrent(branch2);
        });
        prompt->generate();

        sc << prompt;
        after << "Anyways, I'm off to take a break!" << 
        BasicEvent(
            [&speaker, &sfx_victory](Context *ctx, Scene *sc)->void{
                speaker.setBuffer(sfx_victory);
                speaker.play();
            }, [](Context *ctx, Scene *sc)->bool{return true;}
        ) << "Audio was a pain in the a** I will let you know" <<
        BasicEvent(
            [](Context *ctx, Scene *sc)->void{
                ctx->stop();
            }, [](Context *ctx, Scene *sc)->bool{return true;}
        );

        Image img;
        img.loadImage("resources/images/defreimu.png");
        img.setX(-1);
        img.setY(-1);
        img.setWidth(601);
        img.setHeight(401);
        View &game = ctx.getViewsRef().game.view;
        game.addAt(&img, 0);

        ctx.setSpeaker("Orion");

        ctx.start();

        std::cout << "end" << std::endl;
    } catch (std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}
