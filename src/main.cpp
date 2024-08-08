#include <iostream>
#include "math/triangle.h"
#include "math/polygon.h"

using namespace Engine;
int main(){ 
    Vec2d vec = {0,0};
    Vec2d vec2 = {-1,-1};
    std::array<Vec2d, 4> v = {vec, {10, 0}, {10, 10}, {0, 10}};
    std::array<Vec2d, 3> v2 = {vec2, {0, -1}, {21.001, 1}};
    StaticConvexPolygon<4> r1({5,5}, v);
    StaticConvexPolygon<3> r2({5,5}, v2);
    
   // Triangle t1({0,0}, {10, 10}, {0, 10});
    //Vec2d p(0, -1);
    std::cout << r1.Intersects(r2) << "\n";
}