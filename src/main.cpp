#include <numbers>
#include <iostream>

#include "math/triangle.h"
#include "math/polygon.h"
#include "math/matrix.h"
#include "utils/tostrs.h"
#include "world.h"

using namespace Engine;
int main() { 
    Vec2 vec = {0,0};
    Vec2 vec2 = {-1, -1};
    Vec2 vect = {10, 2};
    std::array<Vec2, 4> v = {vec, {10, 0}, {10, 10}, {0, 10}};
    std::array<Vec2, 3> v2 = {vec2, {0, -1}, {21, 1}};
    StaticConvexPolygon<4> r1({5,5}, v);
    StaticConvexPolygon<3> r2({5,5}, v2);
    Matrix3x3 mat1({
        std::array<double,3>{1,4,5},
        {6,2,3},
        {5,3,3}
    });
    Matrix3x3 mat2({
        std::array<double,3>{1,2,3},
        {6,5,4},
        {2,4,4}
    });
    Matrix3x3 result = mat1.Multiply(mat2);
   // Triangle t1({0,0}, {10, 10}, {0, 10});
    //Vec2 p(0, -1);
    Line l1 = {{0,0}, {10, 10}};
    Line l2 = {{10,0}, {0, 10}};
    std::cout << vec2 << "\n" << Matrix3x3::FromTranslationVec(vect).Transform(vec2) << "\n";
    std::cout << l1.GetIntersect(l2).value() << "\n";
    std::cout << r1.Intersects(r2) << "\n\n\n\n";
    std::cout << r1 << "\n";
    r1.Rotate(std::numbers::pi * 0.25);
    std::cout << r1 << "\n";
}