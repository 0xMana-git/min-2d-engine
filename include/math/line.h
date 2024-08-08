#pragma once

#include "vector.h"
#include "../misc/typedefs.h"

#include <cmath>
//Yes its technically a """segment"""
//No i dont care
namespace Engine{
    template<typename T>
    struct _Line {
        using Vec2_t = _Vec2<T>;
        Vec2_t start, end;
        //MAYBE cache this? idk i mean its possible, but idk how useful it will be 
        Vec2_t GetDirection() const {
            return end - start;
        }
        //Note that both of these extend the segment to a line
        T GetDistToPoint(const Vec2_t& p) const {
            Vec2_t dir = GetDirection().GetNormalized();

            Vec2_t point_vec = p - start;
            T dot = dir * point_vec;
            return std::sqrt(point_vec.GetLengthSqr() - dot * dot);

        }
        //Yes there are redundant calcs, no I dont care
        //Returns fraction traced before move vec hits point
        double TraceLineSegmentToPointFrac(const Vec2_t& move_vec, const Vec2_t& point) const {
            T perp_move_distance = std::sqrt(move_vec.GetLengthSqr() - GetDirection().GetNormalized() * move_vec);
            T p_dist = GetDistToPoint(point);
            //yea idgaf at this point
            return ((double)p_dist / (double)perp_move_distance);
        } 
        bool Intersects(const _Line& other) const{

        }


    };


    using Line = _Line<num_t>;

}
