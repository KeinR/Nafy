#include "Shader.h"

#include <iostream>
#include <unordered_map>
#include <string>

#include "../core/glfw.h"
#include "../core/error.h"

#define SAMPLER0_NAME "Tex"
#define MODEL_NAME "model"
#define COLOR_NAME "Color"

namespace cache {
    typedef std::unordered_map<nafy::shader_program_t, nafy::uniform_t> map_t;
    typedef map_t::const_iterator location_t;
    static map_t sampler0;
    static map_t model;
    static map_t color;
    static nafy::uniform_t getFrom(const map_t &map, nafy::shader_program_t shader) {
        location_t loc = map.find(shader);
        std::cout << "RETURN --> " << (loc != map.end() ? loc->second : -1) << std::endl;
        return loc != map.end() ? loc->second : -1;
    }
    static void removeFrom(map_t &map, nafy::shader_program_t shader) {
        location_t loc = map.find(shader);
        if (loc != map.end()) {
            map.erase(loc);
        }
    }
    static void clearFromAll(nafy::shader_program_t shader) {
        removeFrom(sampler0, shader);
        removeFrom(model, shader);
        removeFrom(color, shader);
    }
};

static nafy::uniform_t assertUni(nafy::uniform_t result, const char *name) {
    if (result == -1) {
        throw nafy::gl_error(std::string("Shader does not have requested '") + name + "' uniform");
    }
    return result;
}

nafy::Shader::Shader(shader_program_t shader, uni_t uniforms): shader(shader) {
    if (has(uniforms, uni::sampler0)) {
        uniform_t u = glGetUniformLocation(shader, SAMPLER0_NAME);
        use();
        glUniform1i(u, 0);
        cache::sampler0[shader] = u;
    }
    if (has(uniforms, uni::model)) {
        cache::model[shader] = glGetUniformLocation(shader, MODEL_NAME);
    }
    if (has(uniforms, uni::color)) {
        cache::color[shader] = glGetUniformLocation(shader, COLOR_NAME);
    }
}
nafy::Shader::~Shader() {
    free();
}
bool nafy::Shader::has(uni_t uniforms, uni_t val) {
    return (uniforms & val) == val;
}
void nafy::Shader::free() {
    if (shader) {
        cache::clearFromAll(shader);
        glDeleteProgram(shader);
    }
}
void nafy::Shader::steal(Shader &other) {
    shader = other.shader;
    other.shader = 0;
}
nafy::Shader::Shader(Shader &&other) {
    steal(other);
}
nafy::Shader &nafy::Shader::operator=(Shader &&other) {
    free();
    steal(other);
    return *this;
}

void nafy::Shader::use() const {
    glUseProgram(shader);
}

nafy::shader_program_t nafy::Shader::get() const {
    return shader;
}

nafy::uniform_t nafy::Shader::uniNxSampler0() const noexcept {
    return cache::getFrom(cache::sampler0, shader);
}
nafy::uniform_t nafy::Shader::uniNxModel() const noexcept {
    return cache::getFrom(cache::model, shader);
}
nafy::uniform_t nafy::Shader::uniNxColor() const noexcept {
    return cache::getFrom(cache::color, shader);
}

nafy::uniform_t nafy::Shader::uniSampler0() const {
    return assertUni(uniNxSampler0(), "sampler0; " SAMPLER0_NAME);
}
nafy::uniform_t nafy::Shader::uniModel() const {
    return assertUni(uniNxModel(), "model; " MODEL_NAME);
}
nafy::uniform_t nafy::Shader::uniColor() const {
    return assertUni(uniNxColor(), "color; " COLOR_NAME);
}
