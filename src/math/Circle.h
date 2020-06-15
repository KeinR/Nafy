#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

namespace nafy {
    class Circle {
    public:
        // Members public because there's no special representation,
        // and getters are a pain
        float x;
        float y;
        float radius;
        // Zero initializes
        Circle();
        // Initializer constructor
        Circle(float x, float y, float radius);

        // Initialize new values
        // I'd say it looks cleaner than x = Circle(...)
        void set(float x, float y, float radius);

        bool hasPoint(float xPos, float yPos) const;
        bool overlaps(const Circle &other) const;
    };
}

#endif
