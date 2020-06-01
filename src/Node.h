#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "renderable.h"

namespace nafy {
    class Node: public renderable {
    public:
        typedef renderable *element_t;
    private:
        element_t element;
        bool visible;
    public:
        Node(element_t element);
        void setVisible(bool value);
        bool isVisible() const;
        void setElement(element_t value);
        element_t getElement() const;
        void render() override;
    };
};

#endif
