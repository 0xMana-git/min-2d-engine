#include "math/polygon.h"



using namespace Engine;



bool PolygonBase::Intersects(const PolygonBase& other) const {
    const auto this_tris = std::span(_tri_begin, _tri_end);
    const auto other_tris = std::span(other._tri_begin, other._tri_end);
    for(const Triangle& tri : this_tris) {
        for(const Triangle& other_tri : other_tris) {
            if(tri.Intersects(other_tri))
                return false;    
        }
    }
    return true;
}

