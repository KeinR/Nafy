#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "../shaders/Shader.h"

namespace nafy {
    class Model {
        unsigned int modelLocation;
    public:
        int x;
        int y;
        unsigned int width;
        unsigned int height;
        Model();
        Model(int x, int y, int width, int height);
        void bindShader(shader_t shader, const char *modelName = "model");
        // Note that, unlike some other classes, Model doesn't set the shader itself.
        // You are responsible for keeping track of what shader you bound to it.
        // Also note that a shader must be binded to this Model with bindShader()
        void set();
    };
};

#endif
