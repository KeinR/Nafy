#include "Circle.h"

#include <cmath>

nafy::Circle::Circle(): Circle(0, 0, 0) {
}
nafy::Circle::Circle(float x, float y, float radius): x(x), y(y), radius(radius) {
}
void nafy::Circle::set(float x, float y, float radius) {
    this->x = x;
    this->y = y;
    this->radius = radius;
}
bool nafy::Circle::hasPoint(float xPos, float yPos) const {
    return std::hypot(x - xPos, y - yPos) < radius;
}
bool nafy::Circle::overlaps(const Circle &other) const {
    return std::hypot(x - other.x, y - other.y) < radius + other.radius;
}
