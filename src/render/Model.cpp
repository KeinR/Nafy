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

void nafy::Model::bindShader(shader_t shader, const char *modelName) {
    modelLocation = glGetUniformLocation(shader, modelName);   
}

void nafy::Model::set() {
    glm::mat4 model(1.0f);

    const float xPos = normX(x + 0.375f + (float)width / 2);
    const float yPos = normY(y + 0.375f + (float)height / 2);

    int winWidth, winHeight;
    getWindowSize(&winWidth, &winHeight);
    model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
    model = glm::scale(model, glm::vec3((float)width / winWidth, (float)height / winHeight, 0.0f));

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}
