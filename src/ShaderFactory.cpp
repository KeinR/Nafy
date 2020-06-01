#include "Shader.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include <string>

#include "glfw.h"

#include "env.h"
#include "error.h"

// keyword: filename. Searches for file in resources/shaders
// C-string, std::string, doesn't matter much here I don't think
// static GLuint makeProgram(const char *vertexFilename, const char *fragmentFilename);
static GLuint compileShader(GLenum type, const char *data, int length);
static GLuint linkShaders(GLuint vertObject, GLuint fragObject);

nafy::ShaderFactory::ShaderFactory(): vertLength(0), fragLength(0), vertData(nullptr), fragData(nullptr) {
}
nafy::ShaderFactory::ShaderFactory(const std::string &vertexPath, const std::string &fragmentPath) {
    if (!loadFile(getPath(vertexPath), &vertData, vertLength)) {
        throw error("Failed to load vertex shader file");
    }
    if (!loadFile(getPath(fragmentPath), &fragData, fragLength)) {
        throw error("Failed to load fragment shader file");
    }
}
void nafy::ShaderFactory::copy(const ShaderFactory &other) {
    copyPOD(other);
    dealoc();
    vertData = new char[vertLength];
    fragData = new char[fragLength];
    std::copy(other.vertData, other.vertData + other.vertLength, vertData);
    std::copy(other.fragData, other.fragData + other.fragLength, fragData);
}
void nafy::ShaderFactory::steal(ShaderFactory &other) {
    copyPOD(other);
    dealoc();
    vertData = other.vertData;
    fragData = other.fragData;
    other.vertData = nullptr;
}
void nafy::ShaderFactory::copyPOD(const ShaderFactory &other) {
    vertLength = other.vertLength;
    fragLength = other.fragLength;
}
void nafy::ShaderFactory::dealoc() {
    if (vertData != nullptr) {
        delete[] vertData;
        delete[] fragData;
    }
}
nafy::ShaderFactory::~ShaderFactory() {
    dealoc();
}
nafy::ShaderFactory::ShaderFactory(const ShaderFactory &other) {
    copy(other);
}
nafy::ShaderFactory::ShaderFactory(ShaderFactory &&other) {
    steal(other);
}
nafy::ShaderFactory &nafy::ShaderFactory::operator=(const ShaderFactory &other) {
    copy(other);
    return *this;
}
nafy::ShaderFactory &nafy::ShaderFactory::operator=(ShaderFactory &&other) {
    steal(other);
    return *this;
}
nafy::Shader nafy::ShaderFactory::make() {
    GLuint vertShader = compileShader(GL_VERTEX_SHADER, vertData, vertLength);
    GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fragData, fragLength);
    GLuint shaderProgram = linkShaders(vertShader, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return Shader(shaderProgram);
}

/*
GLuint makeProgram(const char *vertexFilename, const char *fragmentFilename) {
    int vertLen;
    int fragLen;
    char *vertData;
    char *fragData;
    std::string vertexPath = nafy::getPath(std::string("resources/shaders/") + vertexFilename);
    std::string fragmentPath = nafy::getPath(std::string("resources/shaders/") + fragmentFilename);
    if (!loadFile(vertexPath, vertData, vertLen) || !loadFile(fragmentPath, fragData, fragLen)) {
        std::cerr << "WARNING: Errors occurred while loading shader sources [\""
        << vertexPath << "\": " << (vertLen ? "SUCCEED" : "FAIL") << "; \""
        << fragmentPath << "\": " << (fragLen ? "SUCCEED" : "FAIL") <<
        "], aborting shader program build" << std::endl;
        return 0;
    }

    GLuint vertShader = compileShader(GL_VERTEX_SHADER, vertData, vertLen);
    GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fragData, fragLen);

    delete[] vertData;
    delete[] fragData;

    GLuint shaderProgram = linkShaders(vertShader, fragShader);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return shaderProgram;
}
*/

GLuint compileShader(GLenum type, const char *data, int length) {
    GLuint shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &data, &length);
    glCompileShader(shader);
    // Debug
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint linkShaders(GLuint vertObject, GLuint fragObject) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertObject);
    glAttachShader(shaderProgram, fragObject);
    glLinkProgram(shaderProgram);
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shaderProgram;
}

