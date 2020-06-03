#include "Shader.h"

#include <iostream>

#include "glfw.h"

// 0 is silently ignored by glDeleteProgram
nafy::Shader::Shader(): shader(0) {
    std::cout << "Shader default" << std::endl;
}
nafy::Shader::Shader(shader_t shader): shader(shader), refCounter(new int(1)) {
    std::cout << "Shader init" << std::endl;
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
    std::cout << "Shader copy" << std::endl;
    shader = other.shader;
    refCounter = other.refCounter;
    ++(*refCounter);
}
void nafy::Shader::steal(Shader &other) {
    std::cout << "Shader steal" << std::endl;
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
    std::cout << "Shader copy" << std::endl;
    release();
    copy(other);
    return *this;
}
nafy::Shader &nafy::Shader::operator=(Shader &&other) {
    std::cout << "Shader move" << std::endl;
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
    std::cout << "SHADFER EGt" << std::endl;
    std::cout << "shader get " << shader << std::endl;
    return shader;
}
