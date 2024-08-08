#include <iostream>
#include "math/triangle.h"

using namespace Engine;
int main(){ 
    Triangle t1({0,0}, {10, 10}, {0, 10});
    Vec2d p(0, -1);
    std::cout << t1.IsInTriangle(p);
}