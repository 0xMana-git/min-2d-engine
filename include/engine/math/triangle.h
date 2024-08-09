#pragma once 

#include "misc/typedefs.h"
#include "math/vector.h"
#include "math/line.h"

#include <array>



namespace Engine {
    struct Triangle { 
        std::array<Vec2, 3> verts;

        //public:
        bool Intersects(const Triangle& other) const;
        bool IsInTriangle(const Vec2& point) const;
        void Translate(const Vec2& vec);
        Triangle GetTranslated(const Vec2& vec) const;
            
        Triangle(){
            verts.fill(Vec2());
        }
        Triangle(Vec2 a, Vec2 b, Vec2 c){
            verts[0] = a;
            verts[1] = b;
            verts[2] = c;
        }
        Triangle(const Triangle& t) {
            verts = t.verts;
        }
        Triangle(const std::array<Vec2, 3>& verts) {
            this->verts = verts;
        }
            //void BoundedTranslate(const Vec2& vec);
    };
}