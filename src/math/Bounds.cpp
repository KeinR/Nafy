#include "Bounds.h"

#include <limits>

nafy::Bounds::Bounds(): Bounds(0, 0, 0, 0) {
}
nafy::Bounds::Bounds(float minX, float minY, float maxX, float maxY):
    minX(minX), maxX(maxX), minY(minY), maxY(maxY) {
}

void nafy::Bounds::set(float minX, float minY, float maxX, float maxY) {
    this->minX = minX;
    this->maxX = maxX;
    this->minY = minY;
    this->maxY = maxY;
}

void nafy::Bounds::offerX(float x) {
    if (x > maxX) {
        maxX = x;
    } else if (x < minX) {
        minX = x;
    }
}
void nafy::Bounds::offerY(float y) {
    if (y > maxY) {
        maxY = y;
    } else if (y < minY) {
        minY = y;
    }
}

// As opposed to the above, will set the corresponding value
// regardless of the parameter
// void nafy::Bounds::setMinX(float x) {
//     minX = x;
// }
// void nafy::Bounds::setMaxX(float x) {
//     maxX = x;
// }
// void nafy::Bounds::setMinY(float y) {
//     minY = y;
// }
// void nafy::Bounds::setMaxY(float y) {
//     maxY = y;
// }

bool nafy::Bounds::hasPoint(float x, float y) const {
    return minX <= x && x <= maxX && minY <= y && y <= maxY;
}
bool nafy::Bounds::hasPointsOf(const Bounds &other) const {
    return ((minX <= other.minX && other.minX <= maxX) || (minX <= other.maxX && other.maxX <= maxX)) &&
        ((minY <= other.minY && other.minY <= maxY) || (minY <= other.maxY && other.maxY <= maxY));
}
bool nafy::Bounds::overlap(const Bounds &other) const {
    // Of course, x can be in y and/or y in x, hence why we must check both
    return hasPointsOf(other) || other.hasPointsOf(*this);
}
