#include "Shader.h"

#include "glfw.h"

nafy::Shader::Shader(): shader(0) {
}
nafy::Shader::Shader(shader_t shader): shader(shader) {
}
nafy::Shader::~Shader() {
    glDeleteProgram(shader);
}
void nafy::Shader::steal(Shader &other) {
    glDeleteProgram(shader); // 0 is silently ignored
    shader = other.shader;
    other.shader = 0;
}
nafy::Shader::Shader(Shader &&other) {
    steal(other);
}
nafy::Shader &nafy::Shader::operator=(Shader &&other) {
    steal(other);
    return *this;
}
nafy::shader_t nafy::Shader::get() const {
    return shader;
}
