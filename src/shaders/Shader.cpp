#include "Shader.h"

#include <iostream>
#include <unordered_map>
#include <string>

#include "../core/error.h"

// These are the names of the
// corresponding shaders.
// Important because shader uniform
// lookup happens via it's name.
#define SAMPLER0_NAME "Tex"
#define MODEL_NAME "model"
#define COLOR_NAME "Color"

namespace cache {
    // Each context is independent, hence we must seperate the shaders
    // uniform data in each one - even if it is unlikely that there are
    // going to be mutible contexts.
    // As you might expect, this means that shader operations are very dependent
    // on the current context

    typedef std::unordered_map<nafy::Shader::shader_program_t, nafy::uniform_t> map_t;
    typedef map_t::const_iterator location_t;
    struct ctx_maps {
        // All current uniform types
        map_t sampler0;
        map_t model;
        map_t color;
    };

    typedef std::unordered_map<GLFWwindow*, ctx_maps> ctx_map_t;    
    static ctx_map_t maps;

    // Accessor functions

    // Gets the uniform pertaining to `shader` from `map`.
    // If it doesn't actually exist in the map, return -1.
    static nafy::uniform_t getFrom(const map_t &map, nafy::Shader::shader_program_t shader) {
        location_t loc = map.find(shader);
        return loc != map.end() ? loc->second : -1;
    }

    // Removes `shader`'s pertaining data from `map`
    static void removeFrom(map_t &map, nafy::Shader::shader_program_t shader) {
        location_t loc = map.find(shader);
        if (loc != map.end()) {
            map.erase(loc);
        }
    }

    // Removes all of `shader`'s data from the cache for the
    // current context
    static void clearFromAll(nafy::Shader::shader_program_t shader, GLFWwindow *context) {
        ctx_maps &m = maps[context];
        removeFrom(m.sampler0, shader);
        removeFrom(m.model, shader);
        removeFrom(m.color, shader);
    }
}

// Returns true if this Shader has the 
static bool uniHas(nafy::Shader::uni_t uniforms, nafy::Shader::uni_t val) {
    return (uniforms & val) == val;
}

// Assert that `result` isn't -1.
// If it is, throw a gl_error with the given `name` param indicating that
// the shader doesn't have a requested uniform.
static nafy::uniform_t assertUni(nafy::uniform_t result, const char *name) {
    if (result == -1) {
        throw nafy::gl_error(std::string("Shader does not have requested '") + name + "' uniform");
    }
    return result;
}

// Test if there were any errors.
// If so, throw a gl_error with `name` indicating that the shader
// requirement check failed.
static void test(const char *name) {
    const int error = glGetError();
    if (error != GL_NO_ERROR) {
        throw nafy::gl_error(std::string("Shader does not meet uniform requirements for '") + name + "'", error);
    }
}

nafy::Shader::Shader(shader_program_t shader, uni_t uniforms): shader(shader), context(glfwGetCurrentContext()) {
    // Since we need to clear the error state to make sure
    // that any errors that arise are from a failure to
    // check for uniforms, might as well throw an error to
    // let the caller know that there was an OpenGL error
    int error = glGetError();
    if (error != GL_NO_ERROR) {
        throw gl_error("OpenGL error occurred before start of Shader constructor", error);
    }

    // Check through the `uniform` bitfield for different flags.
    // If one's encountered, attempt to initialize that shader uniform.
    // Assert that there are no errors with test(...) so that an exception
    // is thrown if there was a failure somewhere.
    // Note that because values are being added to maps before the checks,
    // happen, and that the first one isn't always the one to fail,
    // the shader uniform cache will be filled with some garbage. This is
    // acceptable, given that a failure for a shader to conform to given
    // uniform requirements should never happen.

    cache::ctx_maps &m = cache::maps[context];

    if (uniHas(uniforms, uni::sampler0)) {
        uniform_t u = glGetUniformLocation(shader, SAMPLER0_NAME);
        use();
        glUniform1i(u, 0);
        m.sampler0[shader] = u;
        test("sampler0");
    }
    if (uniHas(uniforms, uni::model)) {
        m.model[shader] = glGetUniformLocation(shader, MODEL_NAME);
        test("model");
    }
    if (uniHas(uniforms, uni::color)) {
        m.color[shader] = glGetUniformLocation(shader, COLOR_NAME);
        test("color");
    }
}
nafy::Shader::~Shader() {
    free();
}
void nafy::Shader::free() {
    if (shader) {
        cache::clearFromAll(shader, context);
        glDeleteProgram(shader);
    }
}
void nafy::Shader::steal(Shader &other) {
    shader = other.shader;
    context = other.context;
    other.shader = 0;
    other.context = nullptr;
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

nafy::Shader::shader_program_t nafy::Shader::get() const {
    return shader;
}

nafy::uniform_t nafy::Shader::uniNxSampler0() const noexcept {
    return cache::getFrom(cache::maps[context].sampler0, shader);
}
nafy::uniform_t nafy::Shader::uniNxModel() const noexcept {
    return cache::getFrom(cache::maps[context].model, shader);
}
nafy::uniform_t nafy::Shader::uniNxColor() const noexcept {
    return cache::getFrom(cache::maps[context].color, shader);
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
