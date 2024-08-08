#include "math/vector.h"


namespace Engine {


    struct AxisAlignedRect { 
        Vec2d min, max;
        
        AxisAlignedRect(const Vec2d& min, const Vec2d& max){
            this->min = min;
            this->max = max;
        }
    }
}