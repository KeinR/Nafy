#include "Polygon.h"

#include <stdexcept>

// DEBUG
#include <iostream>
#include <fstream>

nafy::Polygon::Polygon(): points(nullptr), lines(nullptr), length(0) {
}
nafy::Polygon::Polygon(float *pointsData, int pointsLen) {
    if (pointsLen % 2) {
        throw std::length_error("nafy::Polygon::Polygon(float*,int): `pointsLen` cannot be odd");
    }
    if (pointsLen <= 0) {
        throw std::length_error("nafy::Polygon::Polygon(float*,int): `pointsLen` cannot be <= 0");
    }
    length = pointsLen / 2;
    points = new Point[length];
    lines = new Line[length];
    for (int i = 0; i < length; i++) {
        const int pi = i * 2;
        const int nextPi = (i + 1) % length * 2;
        lines[i].set(
            pointsData[pi], pointsData[pi+1],
            pointsData[nextPi], pointsData[nextPi+1]
        );
        points[i].set(pointsData[pi], pointsData[pi+1]);
    }
}
nafy::Polygon::Polygon(Polygon &&other) {
    steal(other);
}
nafy::Polygon::~Polygon() {
    free();
}
void nafy::Polygon::free() {
    if (length != 0) {
        delete[] points;
        delete[] lines;
    }
}
void nafy::Polygon::steal(Polygon &other) {
    lines = other.lines;
    points = other.points;
    length = other.length;
    other.length = 0;
}
nafy::Polygon &nafy::Polygon::operator=(Polygon &&other) {
    free();
    steal(other);
    return *this;
}
bool nafy::Polygon::hasPoint(float x, float y) const {
    // Works like this:
    // 
    // Say you have a polygon like so:
    //     _________
    //    /     __  |
    //   /     /  | |
    //  /_____/   |_/
    // 
    // Take a point (+)...
    //     _________
    //    /     __  |
    //   /   + /  | |
    //  /_____/   |_/
    // 
    // And draw a horizontal and vertical line passing through that point
    //     __|_______
    //    /  |   __  |
    // --/---+--/--|-|---
    //  /____|_/   |_/
    //       |
    // 
    // If we take this example here, the portion of the vertical line above the line
    // intersects the polygon once (1), and same for the portion below (1).
    // As for the horizontal line, the left portion intersects the polygon once (1), and
    // the right portion intersects three times (3).
    // What do all these numbers have in common? They're _odd_.
    // 
    // Now let's look at a point outside of the polygon...
    //     _________
    //    /     __  |
    //   /     / +| |
    //  /_____/   |_/
    // 
    // ...and draw vertical and horizontal lines coming out of it
    //     ______|__
    //    /     _|_ |
    // --/-----/-+|-|---
    //  /_____/  ||_/
    //           |
    // 
    // As you can see, top vertical intersects twice (2), bottom vertical zero times (0),
    // left horizontal twice (2), and right horizontal twice (2).
    // As you may have noticed, all of these are _even_.
    // 
    // That's right: If a point is inside a polygon, then lines drawn out of it in one direction will
    // intersect a polygon an odd number of times. This is because a polygon MUST be closed off, so if there
    // are an odd number of ends on one side, you know that they must wrap around and enclose the point.
    // 
    // Such is shown below: It doesn't matter what side is used, I just chose the top side.
    // Time complexity is O(n), with n being the number of lines
    int top = 0;
    for (int i = 0; i < length; i++) {
        if (lines[i].inDomain(x)) {
            float iy = lines[i].solveForY(x, y);
            if (iy > y) {
                top++;
            } else if (iy == y) {
                // If the point at which the ray intersects the
                // line is the point itself, ie the point is on the side
                // of the polygon, let's just say that counts as intersecting.
                return true;
            }
        }
    }
    return top % 2;
}
bool nafy::Polygon::intersects(const Polygon &other) const {
    // Time complexity is O(n^2), with n being the `length` variable.
    for (int i = 0; i < length; i++) {
        for (int f = 0; f < other.length; f++) {
            if (lines[i].intersects(other.lines[f])) {
                return true;
            }
        }
    }
    return false;
}
bool nafy::Polygon::contains(const Polygon &other) const {
    for (int i = 0; i < other.length; i++) {
        if (hasPoint(other.points[i].x, other.points[i].y)) {
            return true;
        }
    }
    return false;
}
bool nafy::Polygon::overlaps(const Polygon &other) const {
    return contains(other) || other.contains(*this);
}
