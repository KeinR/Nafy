#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include <string>

#include "ShaderProgram.h"

namespace nafy {
    class Shader {
        int vertLength;
        int fragLength;
        char *vertData;
        char *fragData;
        inline void copy(const Shader &other);
        inline void steal(Shader &other);
        inline void copyPOD(const Shader &other);
        inline void dealoc();
    public:
        Shader();
        // Throws instance of `nafy::error` if file load failed
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        Shader(const Shader &other);
        Shader(Shader &&other);
        Shader &operator=(const Shader &other);
        Shader &operator=(Shader &&other);
        ~Shader();
        ShaderProgram make();
    };
}

#endif
