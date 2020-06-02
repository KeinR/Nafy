#include "Shader.h"

#include "glfw.h"

// 0 is silently ignored by glDeleteProgram
nafy::Shader::Shader(): shader(0) {
}
nafy::Shader::Shader(shader_t shader): shader(shader), refCounter(new int(1)) {
}
nafy::Shader::~Shader() {
    release();
}
void nafy::Shader::release() {
    if (shader) {
        --(*refCounter);
        if (*refCounter <= 0) {
            delete refCounter;
            glDeleteProgram(shader);
        }
    }
}
void nafy::Shader::copy(const Shader &other) {
    shader = other.shader;
    refCounter = other.refCounter;
    ++(*refCounter);
}
void nafy::Shader::steal(Shader &other) {
    shader = other.shader;
    refCounter = other.refCounter;
    other.shader = 0;
}
nafy::Shader::Shader(const Shader &other) {
    copy(other);
}
nafy::Shader::Shader(Shader &&other) {
    steal(other);
}
nafy::Shader &nafy::Shader::operator=(const Shader &other) {
    release();
    copy(other);
    return *this;
}
nafy::Shader &nafy::Shader::operator=(Shader &&other) {
    release();
    steal(other);
    return *this;
}
void nafy::Shader::reset(shader_t shader) {
    release();
    this->shader = shader;
    if (shader) {
        refCounter = new int(1);
    }
}
nafy::shader_t nafy::Shader::get() const {
    return shader;
}
