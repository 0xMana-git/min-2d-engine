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

double PolygonBase::TraceLineToPolygon(const Line& line, const Vec2& vec) const {
    double frac = std::numeric_limits<double>::max();
    for(const Vec2& vert : verts_view.GetConst()){
        if(!line.IsInLineTrajectory(vec, vert))
                continue;
        frac = std::min(line.TraceLineSegmentToPointFrac(vec, vert), frac);
    }
    return frac;
}

double PolygonBase::TracePolygon(const PolygonBase& other, const Vec2& vec) const {
    double frac = std::numeric_limits<double>::max();
    for(const Line& line : other.segs_view.GetConst()) {
        frac = std::min(TraceLineToPolygon(line, vec), frac);
    }
    //This may be redundant if already got result
    for(const Line& line : segs_view.GetConst()) {
        frac = std::min(other.TraceLineToPolygon(line, -vec), frac);
    }
    return frac;
}