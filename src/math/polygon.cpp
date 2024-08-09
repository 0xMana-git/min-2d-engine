#include "engine/math/polygon.h"



using namespace Engine;



bool PolygonBase::Intersects(const PolygonBase& other) const {
    
    if(!bounding_box.Intersects(other.bounding_box))
        return false;
    for(const Line& seg : segs_view.GetConst()) {
        for(const Line& other_seg : other.segs_view.GetConst()) {
            if(seg.Intersects(other_seg))
                return true;                  
        }
    }


    for(const Triangle& tri : triangles_view.GetConst()) {
        if(tri.IsInTriangle(other.verts_view.GetConst()[0]))
            return true;
    }
    for(const Triangle& tri : other.triangles_view.GetConst()) {

        if(tri.IsInTriangle(verts_view.GetConst()[0]))
            return true;
    }
    return false;
}

