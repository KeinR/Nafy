#include "context.h"

#include <iostream>

#include "error.h"
#include "env.h"

#include "shaders.h"
#include "Font.h"
#include "Text.h"

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

    Font font("Arial.ttf");
    Text text(&font, shaders::sprite);
    text.setText("Hello gamers");
    text.setFontSize(15);
    text.setHexColor(0);
    text.setX(0);
    text.setY(0);


    while (run && engineUp()) {
        std::cout << "_";
        current->run(this);

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
