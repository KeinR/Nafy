#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../env/env.h"

nafy::Model::Model(): modelLocation(0), x(0), y(0), width(0), height(0) {
}
nafy::Model::Model(int x, int y, int width, int height):
    x(x), y(y), width(width), height(height) {
}

void nafy::Model::bindShader(const shader_t &shader) {
    modelLocation = shader->uniModel();   
}

void nafy::Model::set() {
    // Matrix multiplication happens in reverse,
    // so we're actually scaling first, then translating
    glm::mat4 model(1.0f);

    // Translate so that the x/y coords are in the middle
    // of the object (OpenGL style), then convert to quads.
    const float xPos = normX(x + (float)width / 2);
    const float yPos = normY(y + (float)height / 2);
    model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));

    // Convert width and height to OpenGL-readable
    // clamped floats, 0-1
    int winWidth, winHeight;
    getWindowSize(&winWidth, &winHeight);
    model = glm::scale(model, glm::vec3((float)width / winWidth, (float)height / winHeight, 0.0f));

    // Set the computed model to the `modelLocation` in the current shader
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}
