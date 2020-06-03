#ifndef SHADERFACTORY_H_INCLUDED
#define SHADERFACTORY_H_INCLUDED

#include <string>
#include <memory>

#include "Shader.h"

namespace nafy {
    class ShaderFactory {
        int vertLength;
        int fragLength;
        std::shared_ptr<char> vertData;
        std::shared_ptr<char> fragData;
    public:
        ShaderFactory();
        // Throws instance of `nafy::error` if file load failed
        // Note: calls getPath on given paths
        ShaderFactory(const std::string &vertexPath, const std::string &fragmentPath);
        Shader make();
    };
}

#endif
