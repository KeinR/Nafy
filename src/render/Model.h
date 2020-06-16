#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "../shaders/Shader.h"

/*
* Made specifically for setting a mat4 iniform
* in a shader, Model is used to transform verticies
* as they render.
* Note that, unlike some other classes, Model doesn't set the shader itself.
* You are responsible for keeping track of what shader you bound to it.
* Also note that a shader must be binded to this Model with bindShader(...),
* as the object keeps its own copy of the shader's mat4 (model) location.
*/

namespace nafy {
    class Model {
        // The location of the bound shader's mat4 (model)
        uniform_t modelLocation;
    public:
        // Feilds public so as to be a little less of a headache,
        // for there is no possible special representation.
        // (NB: x/y are the top-left hand corner)
        int x;
        int y;
        unsigned int width;
        unsigned int height;
        // Zero initialize
        Model();
        // Initialize with arguments
        Model(int x, int y, int width, int height);
        // Records the model location of the given shader
        // Shader requirements: model
        void bindShader(const shader_t &shader);
        // Load this model's transformation data into
        // the bound shader's uniform located at `modelLocation`.
        // Note that bad things will happen if the bound shader
        // isn't the same one that bindShader(...) was called with last;
        // ie, `modelLocation` must be a location in the bound shader.
        // See file header for details.
        void set();
    };
}

#endif
