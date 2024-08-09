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
#include <algorithm>

namespace Engine {


    struct PolygonBase {
        //this retarded shit is all u get until i find something better, cry about it
        //(u cant leave iterator uninitialized)
        //also these should be only views
        Vec2 origin = Vec2(0,0);
        View<Triangle> triangles;
        View<Vec2> verts;
        View<Line> segments;
        AxisAlignedRect bounding_box;
        //TODO: find a way to optimize the redundant vertices(PROBABLY not a problem but eh who knows)
    public:
        
        bool Intersects(const PolygonBase& other) const;
        virtual double TraceLineToPolygon(const Line& line, const Vec2& vec) const;
        virtual double TracePolygon(const PolygonBase& other, const Vec2& vec) const;
        virtual void Init();
        virtual void InitIterators() = 0;
        void Rotate(double rad);
        void Translate(const Vec2& vec);
    };

    //static probably has WAY better performance but ill make dynamic if I have to
    template<size_t n_verts>
    struct StaticConvexPolygon : PolygonBase {
        //sure I could make loops that can unwrap at compile time, but im lazy
        std::array<Vec2, n_verts> _verts;
        std::array<Triangle, n_verts - 2> _triangles;
        std::array<Line, n_verts> _segments;

        void InitIterators() {
            triangles.Init(_triangles);
            verts.Init(_verts);
            segments.Init(_segments);
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
        

        double TraceLineToPolygon(const Line& line, const Vec2& vec) const {
            double frac = std::numeric_limits<double>::max();
            for(const Vec2& vert : verts){
                if(!line.IsInLineTrajectory(vec, vert))
                        continue;
                frac = std::min(line.TraceLineSegmentToPointFrac(vec, vert), frac);
            }
            return frac;

        }

        template<size_t other_verts_n>
        double TracePolygon(const StaticConvexPolygon<other_verts_n>& other, const Vec2& vec) const {
            double frac = std::numeric_limits<double>::max();
            for(const Line& line : other.segments) {
                frac = std::min(TraceLineToPolygon(line, vec), frac);
            }
            //This may be redundant if already got result
            for(const Line& line : segments) {
                frac = std::min(other.TraceLineToPolygon(line, vec), frac);
            }
            return frac;
        }
        StaticConvexPolygon() {
            _verts.fill({0,0});
            Init();
            InitIterators();
        }
        StaticConvexPolygon(const Vec2& origin, const std::array<Vec2, n_verts>& verts) {
            this->origin = origin;
            this->_verts = _verts;
            Init();
            InitIterators();
        }
        StaticConvexPolygon(const std::array<Vec2, n_verts>& verts) {
            this->origin = {0, 0};
            this->_verts = _verts;
            Init();
            InitIterators();
        }
        StaticConvexPolygon(Vec2&& origin, std::array<Vec2, n_verts>&& verts) : StaticConvexPolygon(std::forward<const Vec2&>(origin), std::forward(verts)) {};
        StaticConvexPolygon(std::array<Vec2, n_verts>&& verts) : StaticConvexPolygon(std::forward(verts)) {};
    };
    using TrianglePolygon = StaticConvexPolygon<3>;
    using QuadConvexPolygon = StaticConvexPolygon<4>;
    struct DynamicConvexPolygon : PolygonBase {
        std::vector<Vec2> _verts;
        std::vector<Triangle> _triangles;
        std::vector<Line> _segments;
        void InitIterators() {
            triangles.Init(_triangles);
            verts.Init(_verts);
            segments.Init(_segments);
        }
        void InitContainers(size_t n_verts) {
            _verts.resize(n_verts);
            _segments.resize(n_verts);
            _triangles.resize(n_verts - 2);
        }
        template<size_t n_verts>
        DynamicConvexPolygon(const StaticConvexPolygon<n_verts>& other) {
            InitContainers(n_verts);
            _verts = other._verts;
            _segments = other._segments;
            _triangles = other._triangles;
        }
        DynamicConvexPolygon(const PolygonBase& other) {
            InitContainers(other.verts.size());
            _verts = std::copy(other.verts._begin, other.verts._end);
            _segments = std::copy(other.segments._begin, other.segments._end);
            _triangles = std::copy(other.triangles._begin, other.triangles._end);
        }


    };
}
