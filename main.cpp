#include "src/story.h"
#include "src/context.h"
#include "src/env.h"
#include "src/error.h"

#include <iostream>

// #include <freetype2/ft2build.h>
// #include FT_FREETYPE_H

int main() {
    try {
        nafy::init(400, 400, "das");
        nafy::scene sc;
        sc.pushText("Hello gamers~");
        nafy::context ctx(&sc);
        ctx.start();
        std::cout << "Free!" << std::endl;
        nafy::deInit();
    } catch (std::exception &e) {
        std::cout << "ERROR: " << e.what() << std::endl;
    }
}
