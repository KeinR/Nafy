#include "src/story.h"
#include "src/context.h"
#include "src/env.h"
#include "src/error.h"
#include "src/ShaderFactory.h"

#include <iostream>

// #include <freetype2/ft2build.h>
// #include FT_FREETYPE_H

int main() {
    try {
        Font font("resources/fonts/Arial.ttf");
        std::cout << "Start, font good = " << font.good() << std::endl;
        nafy::scene sc;
        sc.pushText("Hello gamers~");
        std::cout << "scene done" << std::endl;
        nafy::ShaderFactory shader("resources/shaders/text.vert", "resources/shaders/text.frag");
        std::cout << "make context" << std::endl;
        nafy::context ctx(400, 400, "test", sc, font, shader);
        std::cout << "done context" << std::endl;
        ctx.start();
        std::cout << "Free!" << std::endl;
    } catch (std::exception &e) {
        std::cout << "ERROR: " << e.what() << std::endl;
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
