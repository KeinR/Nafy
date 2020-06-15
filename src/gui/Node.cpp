#include "Node.h"

nafy::Node::Node(): visible(true) {
}
nafy::Node::~Node() {
}
void nafy::Node::setVisible(bool value) {
    visible = value;
}
bool nafy::Node::isVisible() const {
    return visible;
}

template<typename E>
nafy::SubNodeBase<E>::SubNodeBase(E element): Node(), element(element) {
}
template<typename E>
void nafy::SubNodeBase<E>::setElement(E value) {
    element = value;
}
template<typename E>
E nafy::SubNodeBase<E>::getElement() const {
    return element;
}
template<>
bool nafy::SubNodeBase<nafy::renderable*>::isElement(renderable *ptr) const {
    return ptr == element;
}
template<>
bool nafy::SubNodeBase<std::shared_ptr<nafy::renderable>>::isElement(renderable *ptr) const {
    return ptr == element.get();
}
template<typename E>
void nafy::SubNodeBase<E>::render() {
    if (isVisible()) {
        element->render();
    }
}

template class nafy::SubNodeBase<nafy::renderable*>;
template class nafy::SubNodeBase<std::shared_ptr<nafy::renderable>>;
