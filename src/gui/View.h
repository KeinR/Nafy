#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include <vector>

#include "renderable.h"
#include "Node.h"

namespace nafy {
    class View: public renderable {
    public:
        typedef std::vector<Node> array_t;
        typedef array_t::size_type position_t;
    private:
        array_t nodes;
    public:
        View();
        // Returns the index of the new element
        // to be used to later remove the element quickly,
        // or modify it.
        // Do note that the return value is invalidated if there is
        // an addition or removal at any index less than or equal to it
        // which would change the actual index of the element.
        position_t add(renderable *rend);
        // Inserts renderable at `index`
        void addAt(renderable *rend, position_t index);
        // Remove using the iterator returned from the add() function
        // (fast, O(vectorSize - 1 - pos + erased))
        void remove(position_t pos);
        // Remove by searching for element
        // (slow, O(vectorSize + erased))
        void remove(renderable *rend);
        void render() override;

        // Get the underlying node vector for fine tuned re-ordering.
        // Do be careful when modifying the vector, for indices of the
        // elements are returned by the add function.
        array_t &getNodes();
    };
};

#endif
