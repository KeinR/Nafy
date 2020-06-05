#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

// This or shared pointer.
// Honestly, I think this solution is the cleanest, because we're not dealing with
// the pointers themselves, rather just ...

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
        Shader(const Shader &other);
        Shader(Shader &&other);
        Shader &operator=(const Shader &other);
        Shader &operator=(Shader &&other);
        ~Shader();
        void reset(shader_t shader = 0);
        shader_t get() const;
    };
}

#endif
