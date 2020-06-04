#include "Node.h"

template<typename E>
nafy::NodeBase<E>::NodeBase(E element): element(element), visible(true) {
}
template<typename E>
void nafy::NodeBase<E>::setVisible(bool value) {
    visible = value;
}
template<typename E>
bool nafy::NodeBase<E>::isVisible() const {
    return visible;
}
template<typename E>
void nafy::NodeBase<E>::setElement(E value) {
    element = value;
}
template<typename E>
E nafy::NodeBase<E>::getElement() const {
    return element;
}
template<typename E>
void nafy::NodeBase<E>::render() {
    if (visible) {
        element->render();
    }
}

template class nafy::NodeBase<nafy::renderable*>;
template class nafy::NodeBase<std::shared_ptr<nafy::renderable>>;
