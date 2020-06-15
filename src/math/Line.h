#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

/*
* Class wrapper for a simple linear equation.
* Includes special handling for vertical lines.
* However, I would't bet on it handling a slope over a million
* very well.
*/

namespace nafy {
    class Line {
        // In the equation y = m * x + b
        float m;
        float b;
        // aka x min/max
        int domainMin;
        int domainMax;
        // aka y min/max
        int rangeMin;
        int rangeMax;
        // Returns (y - b) / m
        float findX(float y) const;
        // Returns  m * x + b
        float findY(float x) const;
    public:
        // Zero initializes all (except for m, so as to avoid divide-by-zero errors)
        Line();
        // Create line from points
        Line(float x0, float y0, float x1, float y1);
        // Same as above, but later
        void set(float x0, float y0, float x1, float y1);
        // Assumes that inRange(y) == true
        // Solve for x, -> x = (y - b) / m
        // Should the line be horizontal (slope 0), uses
        // fallbackX:
        // If fallbackX < domainMin, returns domainMin.
        // If fallbackX > domainMax, return domainMax.
        // Else, returns fallbackX.
        float solveForX(float y, float fallbackX) const;
        // Assumes that inDomain(x) == true
        // Solve for y, -> y = m * x + b
        // Should the line be vertical (slope infinity), uses
        // fallbackY:
        // If fallbackY < rangeMin, returns rangeMin.
        // If fallbackY > rangeMax, return rangeMax.
        // Else, returns fallbackY.
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
