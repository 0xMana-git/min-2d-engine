#pragma once

#include "misc/typedefs.h"
#include "math/vector.h"
#include "math/line.h"
#include "math/triangle.h"

#include <array>
#include <ranges>
#include <vector>

namespace Engine {


    class PolygonBase {
        //this retarded shit is all u get until i find something better, cry about it
        //(u cant leave iterator uninitialized)
        //also these should be only views
        const Triangle* _tri_begin = 0;
        const Triangle* _tri_end = 0;
        const Vec2d* _vert_begin = 0;
        const Vec2d* _vert_end = 0;
        //TODO: find a way to optimize the redundant vertices(PROBABLY not a problem but eh who knows)
    public:
        bool Intersects(const PolygonBase& other) const;

    };

    //static probably has WAY better performance but ill make dynamic if I have to
    template<size_t n_verts>
    struct StaticConvexPolygon : PolygonBase {
        //sure I could make loops that can unwrap at compile time, but im lazy
        std::array<Vec2d, n_verts> verts;
        std::array<Triangle, n_verts - 2> triangles;

        inline void InitTriangles(){
            std::array<Vec2d, 3> triangle_verts;
            //origin vertex is always constant
            triangle_verts[0] = verts[0];
            //PROBABLY works? idk man
            for(size_t i = 0; i < n_verts - 2; i++){
                triangle_verts[2] = verts[i + 2];
                triangle_verts[1] = verts[i + 1];
                triangles[i] = Triangle(triangle_verts);
            }

        }
        //please unroll..
        template<size_t other_verts_n>
        bool Intersects(const StaticConvexPolygon<other_verts_n>& other) const{
            for(const Triangle& tri : triangles) {
                for(const Triangle& other_tri : other.triangles) {
                    if(tri.Intersects(other_tri))
                        return false;    
                }
            }
        }
        StaticConvexPolygon() {
            verts.fill({0,0});
        }
        StaticConvexPolygon(std::array<Vec2d, n_verts> verts) {
            this->verts = verts;
        }

    };
    class DynamicConvexPolygon : PolygonBase {
        std::vector<Vec2d> verts;

    };
}
