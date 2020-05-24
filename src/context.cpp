#include "context.h"

#include <iostream>

#include "error.h"
#include "env.h"

#include "shaders.h"
#include "text/Font.h"
#include "text/Text.h"

nafy::context::context(scene *root): root(root), current(root), run(false) {
}

void nafy::context::setRoot(scene *root) {
    this->root = root;
}

void nafy::context::setCurrent(scene *current) {
    this->current = current;
    this->current->reset();
}

void nafy::context::revert() {
    setCurrent(root);
}

void nafy::context::start() {
    revert();
    resume();
}

void nafy::context::resume() {
    if (current == nullptr) {
        throw error("`current` must NOT be null");
    }
    run = true;
    std::cout << "Enter?" << std::endl;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Font font("resources/fonts/Arial.ttf");

    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        std::cerr << "ERROR: Failed to init freetype library" << std::endl;
        return;
    }

    Face face = font.mkFace(library, error);
    if (error) {
        std::cerr << "ERROR: Failed to make face " << std::hex << error << std::dec << std::endl;
        return;
    }
    face.setSize(15);

    Text text(face, shaders::text);
    text.setString("The quick brown fox jumped over the lazy dog");
    text.setWrappingWidth(111);
    // text.setLineSpacingMod(1.43);
    text.generate();


    while (run && engineUp()) {
        std::cout << "_";
        current->run(this);
        
        glUseProgram(shaders::text);
        text.render();

        // glClearColor(1, 1, 1, 1.0f);
        // glClearColor(0, 0, 0, 1.0f);
        glClearColor(0.69, 0.52, 0.23, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        text.render();

        glfwSwapBuffers(getWindow());

        glfwPollEvents();
        

    }
}

void nafy::context::stop() {
    run = false;
}

void nafy::context::stopIfCurrent(scene *obj) {
    if (obj == current) {
        stop();
    }
}
