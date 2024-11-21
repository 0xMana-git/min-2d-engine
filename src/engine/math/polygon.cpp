#include "engine/math/polygon.h"



using namespace Engine;



bool PolygonBase::Intersects(const PolygonBase& other) const {
    
    if(!bounding_box.Intersects(other.bounding_box))
        return false;
    
    for(const Vec2& p : verts_view.GetConst()) {
        //MAYBE remove this
        if(!other.bounding_box.IsInBox(p))
            continue;
        if(other.IsInPolygon(p))
            return true;
    }
    for(const Vec2& p : other.verts_view.GetConst()) {
        //MAYBE remove this
        if(!bounding_box.IsInBox(p))
            continue;
        if(IsInPolygon(p))
            return true;
    }

    for(const Line& seg : segs_view.GetConst()) {
        for(const Line& other_seg : other.segs_view.GetConst()) {
            if(seg.Intersects(other_seg))
                return true;                  
        }
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


//https://wrfranklin.org/Research/Short_Notes/pnpoly.html
bool PolygonBase::IsInPolygon(const Vec2& v) const
{
  //const Vec2d& vert0, vert1;
  bool result = false;
  auto span = verts_view.GetConst();
  for(auto vert0 = span.begin(), vert1 = --span.end(); vert0 != span.end(); vert1 = vert0, vert0++) {
    if(vert0->y > v.y == vert1->y > v.y)
        continue;
    double delta = vert1->x - vert0->x;
    if(v.x < delta * (v.y - vert0->y) / delta + vert0->x)
        result = !result;
  }
  return result;
}