#include "shaders.h"

#include <iostream>
#include <fstream>

#include <string>

#include "glfw.h"

#include "env.h"

// keyword: filename. Searches for file in resources/shaders
// C-string, std::string, doesn't matter much here I don't think
static GLuint makeProgram(const char *vertexFilename, const char *fragmentFilename);
static GLuint compileShader(GLenum type, const char *data, int length);
static GLuint linkShaders(GLuint vertObject, GLuint fragObject);
static bool loadFile(const std::string &path, char *&output, int &length);



unsigned int nafy::shaders::sprite;

void nafy::shaders::init() {
    sprite = makeProgram("sprite.vert", "sprite.frag");
}

void nafy::shaders::deInit() {
    glDeleteProgram(sprite);
}



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

bool loadFile(const std::string &path, char *&output, int &length) {
    std::ifstream file(nafy::getPath(path));
    if (!file.good()) {
        std::cerr << "ERROR: Failed to open file \"" << path << "\"" << std::endl;
        file.close();
        return false;
    }
    file.seekg(0, file.end);
    const int len = file.tellg();
    file.seekg(0, file.beg);

    length = len;
    output = new char[len];

    file.read(output, len);

    if (!file.good()) {
        std::cerr << "ERROR: Failed to read from file \"" << path << "\"" << std::endl;
        file.close();
        delete[] output;
        length = 0;
        return false;
    }
    file.close();
    if (!file.good()) { // Not a terribly big deal: we got the data, all is good
        std::cerr << "WARNING: Failed to close file \"" << path << "\"" << std::endl;
    }

    return true;
}
