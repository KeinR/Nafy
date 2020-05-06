#include "src/story.h"
#include "src/context.h"
#include "src/env.h"
#include "src/error.h"

#include <iostream>

int main() {
    try {
        nafy::init();
        // nafy::context ctx;
        nafy::deInit();
    } catch (std::exception &e) {
        std::cout << "ERROR: " << e.what() << std::endl;
    }
}
