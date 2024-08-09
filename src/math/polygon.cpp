#include "engine/math/polygon.h"



using namespace Engine;



bool PolygonBase::Intersects(const PolygonBase& other) const {
    
    if(!bounding_box.Intersects(other.bounding_box))
        return false;
    for(const Line& seg : segments) {
        for(const Line& other_seg : other.segments) {
            if(seg.Intersects(other_seg))
                return true;                  
        }
    }

    for(const Triangle& tri : triangles) {
        if(tri.IsInTriangle(other.verts[0]))
            return true;
    }
    for(const Triangle& tri : other.triangles) {
        if(tri.IsInTriangle(verts[0]))
            return true;
    }
    return false;
}

double PolygonBase::TraceLineToPolygon(const Line& line, const Vec2& vec) const {
    double frac = std::numeric_limits<double>::max();
    for(const Vec2& vert : verts){
        if(!line.IsInLineTrajectory(vec, vert))
                continue;
        frac = std::min(line.TraceLineSegmentToPointFrac(vec, vert), frac);
    }
    return frac;
}

double PolygonBase::TracePolygon(const PolygonBase& other, const Vec2& vec) const {
    double frac = std::numeric_limits<double>::max();
    for(const Line& line : other.segments) {
        frac = std::min(TraceLineToPolygon(line, vec), frac);
    }
    //This may be redundant if already got result
    for(const Line& line : segments) {
        frac = std::min(other.TraceLineToPolygon(line, -vec), frac);
    }
    return frac;
}

void PolygonBase::Init(){
            std::array<Vec2, 3> triangle_verts;
            //origin vertex is always constant
            triangle_verts[0] = verts[0];
            size_t n_verts = verts.size();
            //PROBABLY works? idk man
            for(size_t i = 0; i < n_verts - 2; i++){
                triangle_verts[2] = verts[i + 2];
                triangle_verts[1] = verts[i + 1];
                triangles[i] = Triangle(triangle_verts);
            }
            for(size_t i = 0; i < n_verts - 1; i++){
                segments[i] = Line(verts[i], verts[i + 1]);
            }
            segments[n_verts - 1] = Line(verts[n_verts - 1], verts[0]);

            bounding_box = AxisAlignedRect(verts);

}

void PolygonBase::Translate(const Vec2& vec) {
    origin += vec;
    for(Vec2& vert : verts)
        vert += vec;

    for(Line& seg : segments)
        seg.Translate(vec);

    for(Triangle& tri : triangles)
        tri.Translate(vec);
    bounding_box.Translate(vec);
}

void PolygonBase::Rotate(double rad) {
    //OPERATIONS ARE RIGHT TO LEFT!!!!
    Matrix3x3 transform = Matrix3x3::FromTranslationVec(origin) * Matrix3x3::FromRotation(rad) * Matrix3x3::FromTranslationVec(-origin);
    for(Vec2& vert : verts) {
        vert = transform.Transform(vert);
    }
    Init();
    }