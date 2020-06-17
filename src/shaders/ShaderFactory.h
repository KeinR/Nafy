#ifndef SHADERFACTORY_H_INCLUDED
#define SHADERFACTORY_H_INCLUDED

#include <string>
#include <memory>

#include "Shader.h"

/*
* Used to construct shaders from their raw source code.
* Useful when you need to construct copies of the
* same shader, likely due to working in multible contexts.
*/

namespace nafy {
    class ShaderFactory {
        int vertLength;
        int fragLength;
        std::shared_ptr<char> vertData;
        std::shared_ptr<char> fragData;
    public:
        // Default initialize. Do note that the
        // shader factory will be ill formed, and that
        // any calls to make(...) will fail!
        ShaderFactory();
        // Throws instance of `nafy::error` if file load failed
        // Note: calls getPath on given paths
        ShaderFactory(const std::string &vertexPath, const std::string &fragmentPath);
        // Throws instance of `gl_error` if shader link/compilation failed, or if
        // `vertData` or `fragData` are == nullptr, ie, the factory was default constructed.
        // `uniforms` are parameters for how to initialize the shader
        shader_t make(Shader::uni_t uniforms = Shader::uni::NONE);
    };
}

#endif
