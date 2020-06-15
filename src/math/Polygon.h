#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include "Line.h"
#include "Point.h"

namespace nafy {
    class Polygon {
        Point *points;
        Line *lines;
        int length;
        void free();
        void steal(Polygon &other);
    public:
        Polygon();
        Polygon(float *pointsData, int pointsLen);
        Polygon(Polygon &&other);
        ~Polygon();
        Polygon &operator=(Polygon &&other);
        // Takes a 2D point and returns true if it's contained within
        // this Polygon's bounds
        // Its time complexity is O(n) with n being the number of lines the polygon has.
        // In rare circumastances however, can be as low as O(1) if the point is resting on one
        // of the lines, but that's unlikely enough as to be disregarded. It's O(n).
        bool hasPoint(float x, float y) const;
        // Returns true if this polygon and `other` intersect each-other
        // more efficient than using contains(...) for the same purpose,
        // because contains(...) would require you test them both ways, doubling
        // its time complexity to O(2n^2)
        // The worst time complexity of this function is O(n^2) if the two polygons are not
        // intersecting. If they are, can be as low as O(1).
        bool intersects(const Polygon &other) const;
        // Instead of checking each line to each other, checks against points.
        // Note however that this only checks if `other` is at least in part contained within
        // this polygon - it doesn't check if they are overlapping.
        // It is however superior to intersects(...) however, because intersects only checks if the sides of
        // the polygon overlap.
        // Time complexity is O(n) in the best case (First point in `other` is inside `this`),
        // O(n^2) in the worst case (No points from `other` are found to be in `this`)
        bool contains(const Polygon &other) const;
        // A rather intense function, this calls contains(...) on both polygons to
        // determine if they are actually overlapping. This is needed because intersects(...)
        // only checks if the polygons, well, intersect on their boarders, and contains(...) does that
        // and checks only if _this_ polygon contains `other`.
        // Time complexity is O(n) in the best case (found to be intersecting early),
        // O(2n^2) at the worst (Don't overlap)
        bool overlaps(const Polygon &other) const;
    };
}

#endif
