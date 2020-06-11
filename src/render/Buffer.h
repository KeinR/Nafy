#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include "../gui/renderable.h"

// Holds the raw vertex data

namespace nafy {
    class Buffer: public renderable {
        unsigned int vertices;
        unsigned int indices;

        int countIndices;

        void copy(const Buffer &other);
        void steal(Buffer &other);
        void init();
        void deInit();
    public:
        Buffer();
        Buffer(unsigned int vertices, unsigned int indices, unsigned int countIndices);
        ~Buffer();
        Buffer(Buffer &&other);
        Buffer(const Buffer &other);
        Buffer &operator=(Buffer &&other);
        Buffer &operator=(const Buffer &other);

        void bindVert() const;
        void bindElem() const;
        void bind() const;

        void setVerticies(int count, float *data);
        void setIndices(int count, unsigned int *data);

        void render() override;
    };
}

#endif
