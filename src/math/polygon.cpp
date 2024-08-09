#include "math/polygon.h"



using namespace Engine;



bool PolygonBase::Intersects(const PolygonBase& other) const {
    
    for(const Triangle& tri : triangles_view.GetConst()) {
        for(const Triangle& other_tri : other.triangles_view.GetConst()) {
            if(tri.Intersects(other_tri))
                return false;    
        }
    }
    return true;
}

