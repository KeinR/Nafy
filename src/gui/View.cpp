#include "View.h"

nafy::View::View() {
}
void nafy::View::add(ptr_t rend) {
    nodes.emplace_back(std::make_shared<StaNode>(rend));
}
void nafy::View::add(shared_t rend) {
    nodes.emplace_back(std::make_shared<ManNode>(rend));
}
void nafy::View::addAt(ptr_t rend, position_t index) {
    nodes.insert(nodes.cbegin() + index, std::make_shared<StaNode>(rend));
}
void nafy::View::addAt(shared_t rend, position_t index) {
    nodes.insert(nodes.cbegin() + index, std::make_shared<ManNode>(rend));
}
void nafy::View::remove(position_t pos) {
    nodes.erase(nodes.cbegin() + pos);
}
void nafy::View::remove(ptr_t rend) {
    for (array_t::const_iterator it = nodes.cbegin(); it < nodes.cend(); ++it) {
        if ((*it)->isElement(rend)) {
            nodes.erase(it);
            break;
        }
    }
}
void nafy::View::render() {
    for (node_t &n : nodes) {
        n->render();
    }
}

nafy::View::array_t &nafy::View::getNodes() {
    return nodes;
}
