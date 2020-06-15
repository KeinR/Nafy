#ifndef BOUNDS_H_INCLUDED
#define BOUNDS_H_INCLUDED

// TODO: Setters because minX must be less than maxX

namespace nafy {
    class Bounds {
        bool hasPointsOf(const Bounds &other) const;
    public:
        float minX;
        float maxX;
        float minY;
        float maxY;
        // Default, sets all to zero
        Bounds();
        // Initializer
        Bounds(float minX, float minY, float maxX, float maxY);
        void set(float minX, float minY, float maxX, float maxY);

        // Offer a value to the Bounds object. If it's better than the
        // corresponding value, (minX, minY, ...), it'll be set to that.
        // For example:
        // minX = 0
        // maxX = 0
        // offerX(2)
        // minX = 0
        // maxX = 2
        // offerX(-3)
        // minX = -3
        // maxX = 2
        void offerX(float x);
        void offerY(float y);

        // Determines if a point is contained within the bounds
        // Time complexity is O(1)
        bool hasPoint(float x, float y) const;
        // Intersects or contains...
        // Returns true if the two bounds overlap somewhere
        // Time complexity is O(1)
        bool overlap(const Bounds &other) const;
    };
}

#endif
