#include "math/triangle.h"


using namespace Engine;



bool Triangle::Intersects(const Triangle& other) const {
    for(const Vec2d& p : verts) {
        if(other.IsInTriangle(p))
            return false;
    }
    for(const Vec2d& p : other.verts) {
        if(IsInTriangle(p))
            return false;
    }
    return true;
}

//Credits: https://blackpawn.com/texts/pointinpoly/default.html
bool Triangle::IsInTriangle(const Vec2d& point) const {
    // Compute vectors        
    Vec2d v0 = verts[2] - verts[0];
    Vec2d v1 = verts[1] - verts[0];
    Vec2d v2 = point - verts[0];

    // Compute dot products
    double dot00 = v0 * v0;
    double dot01 = v0 * v1;
    double dot02 = v0 * v2;
    double dot11 = v1 * v1;
    double dot12 = v1 * v2;

    // Compute barycentric coordinates
    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v < 1);
}
void Triangle::Translate(const Vec2d& vec) {
    for(Vec2d& p : verts) {
        p += vec;
    }
}
Triangle Triangle::GetTranslated(const Vec2d& vec) const {
    Triangle newt(*this);
    newt.Translate(vec);
    return newt;
}