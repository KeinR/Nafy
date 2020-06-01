#ifndef DYNODE_H_INCLUDED
#define DYNODE_H_INCLUDED

#include <memory>

#include "renderable.h"

namespace nafy {
    class DyNode: public renderable {
    public:
        typedef std::shared_ptr<renderable> element_t;
    private:
        element_t element;
        bool visible;
    public:
        DyNode(const element_t &element);
        void setVisible(bool value);
        bool isVisible();
        void setElement(const element_t &value);
        element_t getElement();
        void render() override;
    };
};

#endif
