#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

/*
* A simple 2D point
*/

namespace nafy {
    class Point {
    public:
        float x;
        float y;
        Point();
        Point(float x, float y);
        // simply sets the values,
        // really just for convinience
        void set(float x, float y);
    };
}

#endif
