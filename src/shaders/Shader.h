#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <memory>

#include "../core/glfw.h"

/*
* Shaders, useful little programs that process data
* before it's rendered.
* These have to be bound to be used via use(), and when they are,
* they're bound/used globally.
* Since the process of getting shader uniform locations is slow,
* a system of uniform "qualifications" is implemented, where
* a shader will have functions designed to access possible
* uniforms that are stored in a map, with the shader's id
* being used as the key.
* If the required uniform location is not found, depending on the
* function called, an "error" value (-1) is returned, or an
* exception is thrown. Either way, this allows code to do a runtime
* assert that a shader has certain uniforms.
*/

namespace nafy {

    // General type for a uniform value
    typedef int uniform_t;

    class Shader {
    public:
        // Actual shader handle/id type
        typedef unsigned int shader_program_t;
        // enum type for shader qualifications
        typedef int uni_t;
        struct uni {
            // Various shader uniform qualifications.
            // Used to assert at runtime that a shader
            // has certain uniforms.

            // No qualifications
            static constexpr uni_t NONE = 0;
            // Has a sampler2d initialized to active texture 0
            // (name is irrelevant)
            static constexpr uni_t sampler0 = 1 << 0;
            // Has a mat4 named "model"
            static constexpr uni_t model = 1 << 1;
            // Has a vec4 named "color"
            static constexpr uni_t color = 1 << 2;

            // These are all checked in the constructor,
            // and if one failes, an instance of gl_error
            // is thrown
        };
    private:
        // Handle/id of OpenGL shader object
        shader_program_t shader;
        // Pointer to the context it was created in (aka window)
        // This is never actually dereferenced, rather
        // it's used to keep track of which portion of the
        // cache this shader's uniforms are stored in.
        GLFWwindow *context;
        // Frees all managed resources
        void free();
        void steal(Shader &other);
    public:
        // Throws in instance of "gl_error" if the uniform
        // checks based off of `uniforms` fail, or if there
        // was an OpenGL error present at the start of the
        // constructor
        Shader(shader_program_t shader, uni_t uniforms);
        Shader(Shader &&other);
        ~Shader();
        Shader &operator=(Shader &&other);

        // Sets this shader as current
        void use() const;
        // Gets the handle/id of this shader
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

    // The type that a shader will be passed around as - 
    // given that it's kinda' hard to copy them.
    typedef std::shared_ptr<Shader> shader_t;
}

#endif
