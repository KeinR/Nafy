#include "Point.h"

nafy::Point::Point(): x(0), y(0) {
}
nafy::Point::Point(float x, float y): x(x), y(y) {
}
void nafy::Point::set(float x, float y) {
    this->x = x;
    this->y = y;
}
