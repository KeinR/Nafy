#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include <cstddef>

#include "renderable.h"

namespace nafy {
    class Buffer: public renderable {
        unsigned int array;
        unsigned int vertices;
        unsigned int indices;

        int countIndices;

        void steal(Buffer &other);
        void init();
        void deInit();
    public:
        Buffer();
        Buffer(unsigned int array, unsigned int vertices, unsigned int indices, unsigned int countIndices);
        ~Buffer();
        Buffer(Buffer &&other);
        Buffer &operator=(Buffer &&other);

        void bindArr();
        void bindVert();
        void bindElem();
        void bind();

        void setVerticies(int count, float *data);
        void setIndices(int count, unsigned int *data);

        void render() override;

        // Copies element and vertex data into a new Buffer object - nothing more
        Buffer derive();
    };
}

#endif
