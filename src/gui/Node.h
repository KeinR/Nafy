#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <memory>

#include "renderable.h"



namespace nafy {

    class Node: public renderable {
        bool visible;
    public:
        Node();
        virtual ~Node() = 0;
        void setVisible(bool value);
        bool isVisible() const;
        virtual bool isElement(renderable *ptr) const = 0;
        virtual void render() override = 0;
    };

    template<typename E>
    class SubNodeBase: public Node {
        E element;
    public:
        SubNodeBase(E element);
        void setElement(E value);
        E getElement() const;
        bool isElement(renderable *ptr) const override;
        void render() override;
    };

    // Node referencing a renderable with a simple pointer
    typedef SubNodeBase<renderable*> StaNode;
    // Node managing the data it was given through a shared pointer
    typedef SubNodeBase<std::shared_ptr<renderable>> ManNode;
};

#endif
