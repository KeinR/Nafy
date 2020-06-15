#include "Line.h"

#include <cmath>
#include <limits>

nafy::Line::Line(): m(1), b(0), domainMin(0), domainMax(0) {
}
nafy::Line::Line(float x0, float y0, float x1, float y1) {
    set(x0, y0, x1, y1);
}
void nafy::Line::set(float x0, float y0, float x1, float y1) {
    float xDiff = x0 - x1;
    if (xDiff == 0) {
        // There are many ways we could solve vertical lines - however,
        // this method will suffice
        m = std::numeric_limits<float>::infinity();
        b = std::numeric_limits<float>::infinity();
    } else {
        m = (y0 - y1) / xDiff;
        b = y0 - x0 * m;
    }
    // Calculate domain and range min/max
    if (x0 > x1) {
        domainMin = x1;
        domainMax = x0;
    } else {
        domainMin = x0;
        domainMax = x1;
    }
    if (y0 > y1) {
        rangeMin = y1;
        rangeMax = y0;
    } else {
        rangeMin = y0;
        rangeMax = y1;
    }
}
float nafy::Line::findX(float y) const {
    return (y - b) / m;
}
float nafy::Line::findY(float x) const {
    return m * x + b;
}
float nafy::Line::solveForX(float y, float fallbackX) const {
    // Assumes that the y given is in the range
    if (m == 0) {
        if (fallbackX < domainMin) {
            return domainMin;
        } else if (fallbackX > domainMax) {
            return domainMax;
        }
        return fallbackX;
    }
    return findX(y);
}
float nafy::Line::solveForY(float x, float fallbackY) const {
    // Assumes that the x given is in the domain
    if (std::isinf(m)) {
        if (fallbackY < rangeMin) {
            return rangeMin;
        } else if (fallbackY > rangeMax) {
            return rangeMax;
        }
        return fallbackY;
    }
    return findY(x);
}
bool nafy::Line::inDomain(float x) const {
    return domainMin <= x && x <= domainMax;
}
bool nafy::Line::inRange(float y) const {
    return rangeMin <= y && y <= rangeMax;
}
bool nafy::Line::intersects(const Line &line) const {
    // Yes, I am aware that I can omit the else's.
    // I am lazy however, so I'll just let the compiler
    // do it for me.
    if (std::isinf(m)) {
        if (std::isinf(line.m)) {
            // They are both vertical lines, so return true if...
            // They are on the same x, and
            // this contains the start of the line, or
            // this contains the end of the line, or
            // the line contains one of this's points.
            // The last one accounts for `this` being 
            // wrapped inside of the `line`
            return domainMin == line.domainMin && (
                (rangeMin <= line.rangeMin && line.rangeMin <= rangeMax) ||
                (rangeMin <= line.rangeMax && line.rangeMax <= rangeMax) ||
                (line.rangeMin <= rangeMin && rangeMin <= line.rangeMax)
            );
        }
        // this is vertical but line isn't, ergo
        // if the line's y value at this's x-value is
        // within this's range, line and this must be
        // intersecting.
        return inRange(line.findY(domainMin));
    } else if (std::isinf(line.m)) {
        // Same as the last one, but in reverse.
        return line.inRange(findY(line.domainMin));
    } else if (m == 0 && line.m == 0) {
        // Same deal as with when they're both vertical, just
        // flipped on it's side.
        return rangeMin == line.rangeMin && (
            (domainMin <= line.domainMin && line.domainMin <= domainMax) ||
            (domainMin <= line.domainMax && line.domainMax <= domainMax) ||
            (line.domainMin <= domainMin && domainMin <= line.domainMax)
        );
    } else {
        // The classic: both lines are pretty normal, hence their intersection
        // point can be calculated normally
        const float x = (b - line.b) / (m + line.m);
        return inDomain(x) && line.inDomain(x);
    }
}
