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
        //Credits: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
        bool Intersects(const _Line& other) const{
            double s1_x, s1_y, s2_x, s2_y;
            s1_x = end.x - start.x;     
            s1_y = end.y - start.y;
            s2_x = other.end.x - other.start.x;     
            s2_y = other.end.y - other.start.y;

            double s = (-s1_y * (start.x - other.start.x) + s1_x * (start.y - other.start.y)) / (-s2_x * s1_y + s1_x * s2_y);
            double t = ( s2_x * (start.y - other.start.y) - s2_y * (start.x - other.start.x)) / (-s2_x * s1_y + s1_x * s2_y);

            if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
            {
                // Collision detected
                // if (i_x != NULL)
                //     *i_x = start.x + (t * s1_x);
                // if (i_y != NULL)
                //     *i_y = start.y + (t * s1_y);
                //if you want results of intersection
                return true;
            }

            return false; // No collision
        }

        void Translate(const Vec2_t& vec) {
            start += vec;
            end += vec;
        }


    };


    using Line = _Line<num_t>;

}