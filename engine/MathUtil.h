#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <cmath>
#include <iostream>

static inline float SquareFloat(const float& f) {return f*f;}

namespace CMPUT350 {

struct Point2D {
    float x, y;
    Point2D(float x = 0, float y = 0) : x(x), y(y) {}
    double Distance(const Point2D &other) const {
        return sqrt(SquareFloat(x - other.x) + SquareFloat(y - other.y));
    }
    Point2D operator+(const Point2D &other) const {
        return Point2D(x + other.x, y + other.y);
    }
    Point2D operator+(const float &other) const {
        return Point2D(x + other, y+other);
    }
    Point2D operator-(const Point2D &other) const {
        return Point2D(x - other.x, y - other.y);
    }
    Point2D operator-(const float &other) const {
        return Point2D(x - other, y - other);
    }
    Point2D operator*(const float &scalar) const {
        return Point2D(x * scalar, y * scalar);
    }
    Point2D &operator+=(const float &scalar) {
        x += scalar;
        y += scalar;
        return *this;
    }
    Point2D &operator+=(const Point2D &other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point2D &operator-=(const Point2D &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    bool operator==(const Point2D &other) const {
        return (x == other.x && y == other.y);
    }
    Point2D &operator*=(const int &scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Point2D &operator/=(float scalar) {  // int gets truncated and results in zeros/infinities, so we use float
        x /= scalar;
        y /= scalar;
        return *this;
    }
    float operator*(const Point2D &other) const {
        return (x * other.x) + (y * other.y);
    }
    float Dot(Point2D b) const {
        return (*this)*b;
    }
    static float Dot(Point2D a, Point2D b) { // not needed until 1b
        return a*b;
    }
    static float Cross(Point2D a, Point2D b) { // not needed until 1b
        // TODO: write this code
        return 0;
    }
    void Normalize() {
        // Normalizes a given distance
       float lengthsquared = ((Dot(*this)));
       if (lengthsquared != 0.0f){
          float inverse = 1 / sqrt(lengthsquared);
          x *= inverse;
          y *= inverse;
       }
        // We use the idea from quake by finding the squared length, checking for the zero edge case, then using the inverse once to find the scale factor needed to normalize both x and y
    }
};

static std::ostream &operator<<(std::ostream &os, const Point2D &p) {
    // TODO: write this code
    // Gonna hold off on this since I don't know exactly how this should print.
    return os;
}

static Point2D operator*(float number, const Point2D &rhs) {
    return Point2D(rhs.x * number, rhs.y * number);
}

struct Line {
    Point2D p1, p2;

    Line(Point2D p1 = {0, 0}, Point2D p2 = {0, 0}) : p1(p1), p2(p2) {}
    Line(float x1, float y1, float x2, float y2) : p1(x1, y1), p2(x2, y2) {}
    float Length() const {
        return p1.Distance(p2);
    }
    Point2D ClosestPoint(const Point2D &p) const {
        // TODO: write this code
        return p;
    }
    bool Crosses(Line other, Point2D &crossingPoint) const {
        // TODO: write this code
        return false;
    }
};

static std::ostream &operator<<(std::ostream &os, const Line &l) {
    // TODO: write this code
    return os;
}

struct Circle {
    Point2D center;
    float radius;

    Circle(Point2D c = {0, 0}, float r = 0) : center(c), radius(r) {}

    Circle(float x, float y, float r) : center(x, y), radius(r) {}
};

struct Rect {
    Point2D topLeft;
    float width, height;

    Rect(float left, float top, float width, float height)
        : topLeft(Point2D(left, top)), width(width), height(height) {}

    Rect(Point2D tl = {0, 0}, int w = 0, int h = 0) : topLeft(tl), width(w), height(h) {}

    // Creates bounding box around p1 and p2 with positive width/height
    Rect(Point2D p1, Point2D p2)
        : topLeft(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
          width(fabs(p1.x - p2.x)),
          height(fabs(p1.y - p2.y)) {}

    Rect(Point2D center, float radius)
        : topLeft(center.x - radius, center.y - radius), width(2 * radius), height(2 * radius) {}

    Rect &operator|=(const Rect &other) {
        // TODO: Almost identical but swap max and min for top and bottom
        float left = topLeft.x;
        float top = topLeft.y;
        float right = topLeft.x + width;
        float bottom = topLeft.y + height;

        float otherleft = other.topLeft.x;
        float othertop = other.topLeft.y;
        float otherright = other.topLeft.x + other.width;
        float otherbottom = other.topLeft.y + other.height;

        float newleft = std::min(left, otherleft);
        float newtop = std::min(top, othertop);
        float newright = std::max(right, otherright);
        float newbottom = std::max(bottom, otherbottom);

        topLeft.x = newleft;
        topLeft.y = newtop;
        width = newright - topLeft.x;
        height = newbottom - topLeft.y;

        return *this;
    }
    Rect &operator|=(const Point2D &other) {
        // TODO: Same as above but we expand to include one point instead of a whole rectangle
        float left = topLeft.x;
        float top = topLeft.y;
        float right = topLeft.x + width;
        float bottom = topLeft.y + height;

        float newleft = std::min(left,other.x);
        float newtop = std::min(top,other.y);
        float newright = std::max(right,other.x);
        float newbottom = std::max(bottom, other.y);

        topLeft.x = newleft;
        topLeft.y = newtop;
        width = newright - topLeft.x;
        height = newbottom - topLeft.y;

        return (*this);
    }
    Rect &operator|=(const Line &other) {
        // Makes a rectangle big enough to encompass our line
        // if we have Rect r; and Point2D p; then r |= p; means we expand rectangle r so that p is inside of it.

        *this |= other.p1; // expand to encompass point 1
        *this |= other.p2; // expand to encompass point 2

        return *this;
    }
    Rect &operator&=(const Rect &other) {
        // we calculate the shaded reigon of both rectangles overlap
        // for a &= b; we mutate a so that it becomes the intersection of a and b
        float left = topLeft.x;
        float top = topLeft.y;
        float right = topLeft.x + width;
        float bottom = topLeft.y + height;

        float otherleft = other.topLeft.x;
        float othertop = other.topLeft.y;
        float otherright = other.topLeft.x + other.width;
        float otherbottom = other.topLeft.y + other.height;

        float newleft = std::max(left,otherleft);
        float newtop = std::max(top,othertop);
        float newright = std::min(right,otherright);
        float newbottom = std::min(bottom,otherbottom);
        if (newright < newleft || newbottom < newtop){
            width = 0;
            height = 0;
            return (*this);
        }
        topLeft.x = newleft;
        topLeft.y = newtop;
        width = newright - topLeft.x;
        height = newbottom - topLeft.y;

        return *this;
    }
    Rect &operator+=(const Point2D &other) {
        topLeft += other; // changing the offset
        return *this;
    }
    Rect operator+(const Point2D &other) const {
        auto newRect = *this;
        newRect += other;
        return newRect;
    }
    void Inset(int inset) {
        // TODO: Shrinks the Rectangle inward from every side by the same amount

        float left = topLeft.x;
        float top = topLeft.y;

        topLeft.x = left + inset;
        topLeft.y = top + inset;
        width = width - (2*inset);
        height = height - (2*inset);

        // This can return negative dimensions if inset is larger than 1/2 width or height. Potential edge case to consider later.

        return;







    }
    bool IsInside(const Point2D &p) const {
        // Returns True if a box is inside another box else false
        return (
            p.x>= topLeft.x &&
            p.x <= topLeft.x + width &&
            p.y >= topLeft.y &&
            p.y<= topLeft.y + height
        );
    }
};

static std::ostream &operator<<(std::ostream &os, const Rect &l) {
    // TODO: write this code
    return os;
}

}  // namespace CMPUT350

#endif  // MATHUTIL_H
