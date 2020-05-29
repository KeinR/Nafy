#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED


namespace nafy {
    namespace shaders {
        extern shader_t sprite;
        extern shader_t text;
        void init();
        void deInit();
    }
    typedef unsigned int shader_t;
    class ShaderProgram {
        const shader_t shader;
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
    class Shader {
        int vertLength;
        unsigned char *vertData;
        int fragLength;
        unsigned char *fragData;
    public:
        // Throws instance of `nafy::error` if failed
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        ~Shader();
        ShaderProgram make();
    };
}

#endif
