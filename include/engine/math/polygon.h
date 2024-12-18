#pragma once

#include "engine/misc/typedefs.h"
#include "engine/math/vector.h"
#include "engine/math/line.h"
#include "engine/math/triangle.h"
#include "engine/math/rect.h"
#include "engine/math/matrix.h"
#include "engine/utils/view.h"

#include <array>
#include <ranges>
#include <vector>
#include <cmath>
#include <limits>

namespace Engine {


    struct PolygonBase {
        //this retarded shit is all u get until i find something better, cry about it
        //(u cant leave iterator uninitialized)
        //also these should be only views
        Vec2 origin = Vec2(0,0);
        View<Triangle> triangles_view;
        View<Vec2> verts_view;
        View<Line> segs_view;
        AxisAlignedRect bounding_box;
        //TODO: find a way to optimize the redundant vertices(PROBABLY not a problem but eh who knows)
    public:
        bool Intersects(const PolygonBase& other) const;
        bool IsInPolygon(const Vec2& point) const;
        virtual double TraceLineToPolygon(const Line& line, const Vec2& vec) const;
        virtual double TracePolygon(const PolygonBase& other, const Vec2& vec) const;
        //void Rotate(double rad);
        //void Translate(const Vec2& vec);
    };
    
    //static probably has WAY better performance but ill make dynamic if I have to
    template<size_t n_verts>
    struct StaticConvexPolygon : PolygonBase {
        //sure I could make loops that can unwrap at compile time, but im lazy
        std::array<Vec2, n_verts> verts;
        std::array<Triangle, n_verts - 2> triangles;
        std::array<Line, n_verts> segments;
        using TrianglePolygon = StaticConvexPolygon<3>;
        using QuadConvexPolygon = StaticConvexPolygon<4>;

        inline void Init(){
            std::array<Vec2, 3> triangle_verts;
            //origin vertex is always constant
            triangle_verts[0] = verts[0];
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

        inline void InitIterators() {
            triangles_view.Init(triangles);
            verts_view.Init(verts);
            segs_view.Init(segments);
        }
        //Its probably quicker to translate everything instead of calling init again so
        void Translate(const Vec2& vec) {
            origin += vec;
            for(Vec2& vert : verts)
                vert += vec;

            for(Line& seg : segments)
                seg.Translate(vec);

            for(Triangle& tri : triangles)
                tri.Translate(vec);
            bounding_box.Translate(vec);
        }
        void Rotate(double rad) {
            //OPERATIONS ARE RIGHT TO LEFT!!!!
            Matrix3x3 transform = Matrix3x3::FromTranslationVec(origin) * Matrix3x3::FromRotation(rad) * Matrix3x3::FromTranslationVec(-origin);
            for(Vec2& vert : verts) {
                vert = transform.Transform(vert);
            }
            Init();
        }
        //please unroll..
        template<size_t other_verts_n>
        bool Intersects(const StaticConvexPolygon<other_verts_n>& other) const {
            if(!bounding_box.Intersects(other.bounding_box))
                return false;
            for(const Line& seg : segments) {
                for(const Line& other_seg : other.segments) {
                    if(seg.Intersects(other_seg))
                        return true;      
                }
            }
            //i THINK i only need to check one of the verts? idk
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
        
        
        bool Intersects(const PolygonBase& other) const {
            if(!bounding_box.Intersects(other.bounding_box))
                return false;
            for(const Line& seg : segments) {
                for(const Line& other_seg : other.segs_view.GetConst()) {
                    if(seg.Intersects(other_seg))
                        return true;      
                }
            }
            //i THINK i only need to check one of the verts? idk
            for(const Triangle& tri : triangles) {
                if(tri.IsInTriangle(other.verts_view.GetConst()[0]))
                    return true;
            }
            for(const Triangle& tri : other.triangles_view.GetConst()) {
                if(tri.IsInTriangle(verts[0]))
                    return true;
            }
            return false;
        }
        //ambiguous name
        double StaticTraceLineToPolygon(const Line& line, const Vec2& vec) const {
            double frac = std::numeric_limits<double>::max();
            for(const Vec2& vert : verts){
                if(!line.IsInLineTrajectory(vec, vert))
                    continue;
                frac = std::min(line.TraceLineSegmentToPointFrac(vec, vert), frac);
            }
            return frac;

        }

        double TraceLineToPolygon(const Line& line, const Vec2& vec) const {
            return StaticTraceLineToPolygon(line, vec);
        }
        template<size_t other_verts_n>
        double StaticTracePolygon(const StaticConvexPolygon<other_verts_n>& other, const Vec2& vec) const {
            double frac = std::numeric_limits<double>::max();
            for(const Line& line : other.segments) {
                frac = std::min(StaticTraceLineToPolygon(line, vec), frac);
            }
            //This may be redundant if already got result
            for(const Line& line : segments) {
                frac = std::min(other.StaticTraceLineToPolygon(line, vec), frac);
            }
            return frac;
        }

        double TracePolygon(const PolygonBase& other, const Vec2& vec) const {
            if(typeid(other) == typeid(TrianglePolygon))
                return StaticTracePolygon(static_cast<const TrianglePolygon&>(other), vec);
            if(typeid(other) == typeid(QuadConvexPolygon))
                return StaticTracePolygon(static_cast<const QuadConvexPolygon&>(other), vec);
            throw std::exception();
        }
        StaticConvexPolygon() {
            verts.fill({0,0});
            Init();
            InitIterators();
        }
        StaticConvexPolygon(const Vec2& origin, const std::array<Vec2, n_verts>& verts) {
            this->origin = origin;
            this->verts = verts;
            Init();
            InitIterators();
        }
        StaticConvexPolygon(const std::array<Vec2, n_verts>& verts) {
            this->origin = {0, 0};
            this->verts = verts;
            Init();
            InitIterators();
        }
        StaticConvexPolygon(Vec2&& origin, std::array<Vec2, n_verts>&& verts) : StaticConvexPolygon(std::forward<const Vec2&>(origin), std::forward(verts)) {};
        StaticConvexPolygon(std::array<Vec2, n_verts>&& verts) : StaticConvexPolygon(std::forward(verts)) {};
    };
    using TrianglePolygon = StaticConvexPolygon<3>;
    using QuadConvexPolygon = StaticConvexPolygon<4>;
    class DynamicConvexPolygon : PolygonBase {
        std::vector<Vec2> verts;

    };
}
