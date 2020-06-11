#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include "../gui/renderable.h"

#include <cstddef>

// Holds the raw vertex data

namespace nafy {
    class Buffer: public renderable {
    public:
        typedef void(*initfunc_t)(Buffer &buffer);
    private:
        unsigned int array;
        unsigned int vertices;
        unsigned int indices;

        int countIndices;

        // So that we can copy this
        initfunc_t initFunc;

        void copy(const Buffer &other);
        void steal(Buffer &other);
        void gen();
        void deGen();
    public:
        // NB: If copied while init is nullptr, the copied will be ill-formed!
        Buffer(initfunc_t init = nullptr);
        ~Buffer();
        Buffer(Buffer &&other);
        Buffer(const Buffer &other);
        Buffer &operator=(Buffer &&other);
        Buffer &operator=(const Buffer &other);

        void setInit(initfunc_t initFunc);
        void init();

        void bindArray() const;
        void bindVert() const;
        void bindElem() const;
        void bind() const;

        void setVerticies(int count, float *data);
        void setIndices(int count, unsigned int *data);
        void setParam(unsigned int index, int size, std::size_t stride, const void *offset);

        void render() override;
    };
}

#endif
