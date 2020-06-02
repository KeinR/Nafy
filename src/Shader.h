#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

namespace nafy {
    typedef unsigned int shader_t;
    class Shader {
        shader_t shader;
        int *refCounter;
        inline void release();
        inline void copy(const Shader &other);
        inline void steal(Shader &other);
    public:
        Shader();
        Shader(shader_t shader);
        Shader(const Shader &other) = delete;
        Shader(Shader &&other);
        Shader &operator=(const Shader &other) = delete;
        Shader &operator=(Shader &&other);
        ~Shader();
        void reset(shader_t shader = 0);
        shader_t get() const;
    };
}

#endif
