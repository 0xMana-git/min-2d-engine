#pragma once

#include "math/vector.hpp"
#include "misc/typedefs.hpp"

#include <cmath>
#include <optional>
//Yes its technically a """segment"""
//No i dont care
namespace Engine{
    constexpr double EPS = 1E-16;
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
            Vec2_t line_dir = GetDirection().GetNormalized();
            //T parallel_dist = line_dir * move_vec;
            T p_perp_dist = GetDistToPoint(point);
            T dot = line_dir * move_vec;

            T perp_move_distance = std::sqrt(move_vec.GetLengthSqr() - dot * dot);
            
            //yea idgaf at this point
            return ((double)p_perp_dist / (double)perp_move_distance);
        } 

        //credits: https://cp-algorithms.com/geometry/segments-intersection.html
        inline static bool _intersect_1d(T a, T b, T c, T d) 
        {
            if (a > b)
                std::swap(a, b);
            if (c > d)
                std::swap(c, d);
            return std::max(a, c) <= std::min(b, d) + EPS;
        }

        //Credits: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
        //IMPORTANT: ORDER MATTERS!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //when 2 lines are collinear, the intersect is defined as the point of the other line that is closer to this start
        inline std::optional<Vec2_t> GetIntersect(const _Line& other) const{

            //bounding box check, i REALLY hope this solve collinear edgecase
            if(!_intersect_1d(start.x, end.x, other.start.x, other.end.x)) 
                return {};
            if(!_intersect_1d(start.y, end.y, other.start.y, other.end.y))
                return {};

            Vec2_t this_delta = end - start;
            Vec2_t other_delta = other.end - other.start;
            // If the direction cross product = 0, they are facing the same direction
            double dir_cross = this_delta.Cross(other_delta);
            double cross_inv = 1 / dir_cross;

            
            Vec2_t this_other_delta = other.start - start;
            //if delta vec and direction(one of the vecs) cross = 0, one of the direction is in the same direction as the delta vec
            double dir_delta_cross = this_other_delta.Cross(this_delta);

            bool dir_parallel = std::abs(dir_cross) < 0;
            bool dir_delta_parallel = std::abs(dir_delta_cross) < 0;
            
            //parallel but not collinear
            if(dir_parallel && !(dir_delta_parallel))
                return {};
            
            //if collinear
            //its DEFINITELY joined due to bbox check
            if(dir_parallel){
                //there is a better way but im sick of this fuckin shit
                Vec2_t start_to_other_end = other.end - start;
                Vec2_t& start_to_other_start = this_other_delta;
                double start_dp = this_delta * start_to_other_start;
                
                if(start_dp < 0)
                    return other.end;
                //in this case, both have positive dp because any other case is impossible

                //if other start is closer
                if(start_to_other_end.GetLengthSqr() < start_to_other_start.GetLengthSqr())
                    return other.end;
                return other.start;

            }
                
            
            
            //there should be a check for disjoining vecs, but i THINK bbox check handles it
            double u = dir_delta_cross * cross_inv;

            //the 1 checks are redundant due to bbox but im keeping it
            if(u < 0 || u > 1) 
                return {};
            double t = this_other_delta.Cross(other_delta) * cross_inv;
            if(t < 0 || t > 1) 
                return {};
            return Vec2_t(start + (this_delta * t)); // No collision
        }
        bool IsInLineTrajectory(const Vec2_t& move_vec, const Vec2_t& point) const {
            Vec2_t sd = point - start, ed = point - end;
            return move_vec.Cross(sd) * move_vec.Cross(ed) < 0;
        }

        
        bool Intersects(const _Line& other) const{
            return GetIntersect(other).has_value();
        }
        void Translate(const Vec2_t& vec) {
            start += vec;
            end += vec;
        }
        std::string to_string() const {
            return "start: " + start.to_string() + "    end: " + end.to_string();
        }


    };


    using Line = _Line<num_t>;

}
