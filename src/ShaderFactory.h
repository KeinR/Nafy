#ifndef SHADERFACTORY_H_INCLUDED
#define SHADERFACTORY_H_INCLUDED

#include <string>

#include "Shader.h"

namespace nafy {
    class ShaderFactory {
        int vertLength;
        int fragLength;
        char *vertData;
        char *fragData;
        inline void copy(const ShaderFactory &other);
        inline void steal(ShaderFactory &other);
        inline void copyPOD(const ShaderFactory &other);
        inline void dealoc();
    public:
        ShaderFactory();
        // Throws instance of `nafy::error` if file load failed
        // Note: calls getPath on given paths
        ShaderFactory(const std::string &vertexPath, const std::string &fragmentPath);
        ShaderFactory(const ShaderFactory &other);
        ShaderFactory(ShaderFactory &&other);
        ShaderFactory &operator=(const ShaderFactory &other);
        ShaderFactory &operator=(ShaderFactory &&other);
        ~ShaderFactory();
        Shader make();
    };
}

#endif
