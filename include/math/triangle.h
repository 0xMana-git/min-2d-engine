#pragma once 

#include "misc/typedefs.h"
#include "math/vector.h"
#include "math/line.h"

#include <array>



namespace Engine {
    struct Triangle { 
        std::array<Vec2d, 3> verts;

        //public:
        bool Intersects(const Triangle& other) const;
        bool IsInTriangle(const Vec2d& point) const;
        void Translate(const Vec2d& vec);
        Triangle GetTranslated(const Vec2d& vec) const;
            
        Triangle(){
            verts.fill(Vec2d());
        }
        Triangle(Vec2d a, Vec2d b, Vec2d c){
            verts[0] = a;
            verts[1] = b;
            verts[2] = c;
        }
        Triangle(const Triangle& t) {
            verts = t.verts;
        }
        Triangle(const std::array<Vec2d, 3>& verts) {
            this->verts = verts;
        }
            //void BoundedTranslate(const Vec2d& vec);
    };
}