#include "ShaderFactory.h"

#include <iostream>
#include <fstream>

#include <string>

#include "../core/glfw.h"

#include "../env/env.h"
#include "../core/error.h"

// Compiles the given shader and returns a handle to the OpenGL object.
// Throws an instance of gl_error if failed
static GLuint compileShader(GLenum type, const char *data, int length);
// Links two shaders (vertex and fragment) and returns the resulting
// shader program.
// Throws an instance of gl_error if failed
static GLuint linkShaders(GLuint vertObject, GLuint fragObject);

nafy::ShaderFactory::ShaderFactory(): vertLength(0), fragLength(0) {
}
nafy::ShaderFactory::ShaderFactory(const std::string &vertexPath, const std::string &fragmentPath) {
    char *vd;
    if (!loadFile(getPath(vertexPath), &vd, vertLength)) {
        throw error("Failed to load vertex shader file");
    }
    char *fd;
    if (!loadFile(getPath(fragmentPath), &fd, fragLength)) {
        throw error("Failed to load fragment shader file");
    }
    vertData.reset(vd);
    fragData.reset(fd);
}
nafy::shader_t nafy::ShaderFactory::make(Shader::uni_t uniforms) {
    if (!vertData || !fragData) {
        throw gl_error("nafy::ShaderFactory::make: Not initialized");
    }
    GLuint vertShader = compileShader(GL_VERTEX_SHADER, vertData.get(), vertLength);
    GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fragData.get(), fragLength);
    GLuint shaderProgram = linkShaders(vertShader, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return std::make_shared<Shader>(shaderProgram, uniforms);
}

GLuint compileShader(GLenum type, const char *data, int length) {
    GLuint shader;
    shader = glCreateShader(type);
    // Load shader source code
    glShaderSource(shader, 1, &data, &length);
    // Compile code
    glCompileShader(shader);
    // Check for failure
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char *log = new char[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::string errMsg(
            "ShaderFactory::make::compileShader(" +
            std::to_string(type) +
            ")::SHADER::COMPILATION_FAILED:\n" +
            log
        );
        delete[] log;
        throw nafy::gl_error(errMsg);
    }
    return shader;
}

GLuint linkShaders(GLuint vertObject, GLuint fragObject) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertObject);
    glAttachShader(shaderProgram, fragObject);
    glLinkProgram(shaderProgram);
    // Check for failure
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char *log = new char[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        std::string errMsg("ShaderFactory::make::linkShaders::SHADER_PROGRAM::COMPILATION_FAILED:\n");
        errMsg += log;
        delete[] log;
        throw nafy::gl_error(errMsg);
    }
    return shaderProgram;
}

