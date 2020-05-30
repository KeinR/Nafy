#include "ShaderProgram.h"

#include "glfw.h"

nafy::ShaderProgram::ShaderProgram(): shader(0) {
}
nafy::ShaderProgram::ShaderProgram(shader_t shader): shader(shader) {
}
nafy::ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shader);
}
void nafy::ShaderProgram::steal(ShaderProgram &other) {
    glDeleteProgram(shader); // 0 is silently ignored
    shader = other.shader;
    other.shader = 0;
}
nafy::ShaderProgram::ShaderProgram(ShaderProgram &&other) {
    steal(other);
}
nafy::ShaderProgram &nafy::ShaderProgram::operator=(ShaderProgram &&other) {
    steal(other);
    return *this;
}
nafy::shader_t nafy::ShaderProgram::get() const {
    return shader;
}
