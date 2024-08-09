#pragma once

#include "math/vector.h"


#include <string>

#define OVERLOAD_SHIFT_STREAM_OPERATOR(_Ty) ostream& operator<<(ostream &strm, const _Ty& obj) { return internal_operator_shift<_Ty>(strm, obj); }
namespace std {
    template <typename T>
    constexpr string to_string(const Engine::_Vec2<T>& obj) {
        return "x: " + to_string(obj.x) + ", y: " + to_string(obj.y); 
    }
    
    template<size_t n_verts>
    constexpr string to_string(const Engine::StaticConvexPolygon<n_verts>& obj) {
        string res = to_string(n_verts) + " sided convex polygon\n";
        for(size_t i = 0; i < n_verts; i++) {
            res += "vert " + to_string(i) + ": " + to_string(obj.verts[i]) + "\n";
        };
        return res;
    }


    template <typename T>
    ostream& internal_operator_shift(ostream &strm, const T& obj) {
        return strm << to_string(obj);
    }

    //sorry.....
    template<size_t n_verts>
    OVERLOAD_SHIFT_STREAM_OPERATOR(Engine::StaticConvexPolygon<n_verts>);

    template <typename T>
    OVERLOAD_SHIFT_STREAM_OPERATOR(Engine::_Vec2<T>);


}
