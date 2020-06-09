#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <memory>

namespace nafy {
    typedef unsigned int shader_program_t;
    typedef int uniform_t;
    class Shader {
    public:
        typedef int uni_t;
        struct uni {
            static constexpr uni_t NONE = 0;
            static constexpr uni_t sampler0 = 1 << 0;
            static constexpr uni_t model = 1 << 1;
            static constexpr uni_t color = 1 << 2;
        };
    private:
        shader_program_t shader;
        void free();
        void steal(Shader &other);
        bool has(uni_t uniforms, uni_t val);
    public:
        Shader(shader_program_t shader, uni_t uniforms);
        Shader(Shader &&other);
        Shader &operator=(Shader &&other);
        ~Shader();

        void use() const;
        shader_program_t get() const;

        // Uniform gets, if fail return -1
        uniform_t uniNxSampler0() const noexcept;
        uniform_t uniNxModel() const noexcept;
        uniform_t uniNxColor() const noexcept;

        // Uniform gets, if fail throw an instance of `gl_error`
        uniform_t uniSampler0() const;
        uniform_t uniModel() const;
        uniform_t uniColor() const;
    };
    typedef std::shared_ptr<Shader> shader_t;
}

#endif
