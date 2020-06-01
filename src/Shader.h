#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

namespace nafy {
    typedef unsigned int shader_t;
    class Shader {
        shader_t shader;
        inline void steal(Shader &other);
    public:
        Shader();
        Shader(shader_t shader);
        Shader(const Shader &other) = delete;
        Shader(Shader &&other);
        Shader &operator=(const Shader &other) = delete;
        Shader &operator=(Shader &&other);
        ~Shader();
        shader_t get() const;
    };
}

#endif
