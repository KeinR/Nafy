#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

/*
* Class wrapper for a simple linear equation.
* Doesn't handle vertical lines very well.
*/

namespace nafy {
    class Line {
        float m;
        float b;
        int domainMin;
        int domainMax;
        int rangeMin;
        int rangeMax;
        float findX(float y) const;
        float findY(float x) const;
    public:
        // Zero initializes all (except for m, so as to avoid divide-by-zero errors)
        Line();
        // Create line from points
        Line(float x0, float y0, float x1, float y1);
        // Same as above, but later
        void set(float x0, float y0, float x1, float y1);
        // Solve for x, -> x = (y - b) / m
        float solveForX(float y, float fallbackX) const;
        // Solve for y, -> y = m * x + b
        float solveForY(float x, float fallbackY) const;
        // Gets if `x` is in this line's domain
        bool inDomain(float x) const;
        // Gets if `y` is in this line's range
        bool inRange(float y) const;
        // Gets if two lines intersect in their respective domains
        bool intersects(const Line &line) const;
    };
}

#endif
