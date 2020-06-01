#include "View.h"

nafy::View::View() {
}
nafy::View::position_t nafy::View::add(renderable *rend) {
    nodes.emplace_back(rend);
    return nodes.size() - 1;
}
void nafy::View::remove(position_t pos) {
    nodes.erase(nodes.cbegin() + pos);
}
void nafy::View::remove(renderable *rend) {
    for (array_t::const_iterator it = nodes.cbegin(); it < nodes.cend(); ++it) {
        if (it->getElement() == rend) {
            nodes.erase(it);
            break;
        }
    }
}
void nafy::View::render() {
    for (Node &n : nodes) {
        n.render();
    }
}

nafy::View::array_t &nafy::View::getNodes() {
    return nodes;
}
