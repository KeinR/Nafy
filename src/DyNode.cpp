#include "DyNode.h"

nafy::DyNode::DyNode(const element_t &element): element(element), visible(true) {
}
void nafy::DyNode::setVisible(bool value) {
    visible = value;
}
bool nafy::DyNode::isVisible() {
    return visible;
}
void nafy::DyNode::setElement(const element_t &value) {
    element = value;
}
element_t nafy::DyNode::getElement() {
    return element;
}
void nafy::DyNode::render() {
    if (visible) {
        element->render();
    }
}
