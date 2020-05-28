#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED


namespace nafy {
    typedef unsigned int shader_t;
    namespace shaders {
        extern shader sprite;
        extern shader text;
        void init();
        void deInit();
    }
}

#endif
