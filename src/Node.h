#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <memory>

#include "renderable.h"

namespace nafy {
    template<typename E>
    class NodeBase: public renderable {
        E element;
        bool visible;
    public:
        NodeBase(E element);
        void setVisible(bool value);
        bool isVisible() const;
        void setElement(E value);
        E getElement() const;
        void render() override;
    };

    typedef NodeBase<renderable*> Node;
    typedef NodeBase<std::shared_ptr<renderable>> DyNode;
};

#endif
