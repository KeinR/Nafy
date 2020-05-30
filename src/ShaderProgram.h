#ifndef SHADERPROGRAM_H_INCLUDED
#define SHADERPROGRAM_H_INCLUDED

namespace nafy {
    typedef unsigned int shader_t;
    class ShaderProgram {
        shader_t shader;
        inline void steal(ShaderProgram &other);
    public:
        ShaderProgram();
        ShaderProgram(shader_t shader);
        ShaderProgram(const ShaderProgram &other) = delete;
        ShaderProgram(ShaderProgram &&other);
        ShaderProgram &operator=(const ShaderProgram &other) = delete;
        ShaderProgram &operator=(ShaderProgram &&other);
        ~ShaderProgram();
        shader_t get() const;
    };
}

#endif
