#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include <cstddef>

namespace nafy {
    class Buffer {
        unsigned int array;
        unsigned int vertices;
        unsigned int indices;

        void steal(Buffer &other);
        void init();
        void deInit();
    public:
        Buffer();
        Buffer(unsigned int array, unsigned int vertices, unsigned int indices);
        ~Buffer();
        Buffer(Buffer &&other);
        Buffer &operator=(Buffer &&other);

        void bindArr();
        void bindVert();
        void bindElem();
        void bind();

        void setVerticies(std::size_t size, float *data);
        void setIndices(std::size_t size, unsigned int *data);

        // Copies element and vertex data into a new Buffer object - nothing more
        Buffer derive();
    };
}

#endif
