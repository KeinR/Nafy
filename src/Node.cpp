#include "Node.h"

nafy::Node::Node(element_t element): element(element), visible(true) {
}
void nafy::Node::setVisible(bool value) {
    visible = value;
}
bool nafy::Node::isVisible() const {
    return visible;
}
void nafy::Node::setElement(element_t value) {
    element = value;
}
nafy::Node::element_t nafy::Node::getElement() const {
    return element;
}
void nafy::Node::render() {
    if (visible) {
        element->render();
    }
}
