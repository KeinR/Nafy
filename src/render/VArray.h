#ifndef VERTEXARRAY_H_INCLUDED
#define VERTEXARRAY_H_INCLUDED

#include "../core/glfw.h"

#include <cstddef>

// Vertex arrays instruct on how to interpret the data in (vertex) Buffers

namespace nafy {
    class VArray {
        unsigned int handle;
        void steal(VArray &other);
    public:
        // Generate new
        VArray();
        // Takes ownership of provided handle
        VArray(unsigned int id);
        VArray(VArray &&other);
        ~VArray();
        VArray &operator=(VArray &&other);

        void bind() const;
        void setParam(unsigned int index, int size, std::size_t stride, const void *offset);
    };
}

#endif
