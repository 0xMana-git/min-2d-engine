#pragma once

#include "math/vector.h"
#include "misc/typedefs.h"

#include <cmath>
#include <optional>
// Yes its technically a """segment"""
// No i dont care
namespace Engine
{
    template <typename T>
    struct _Line
    {
    public:
        using Vec2_t = _Vec2<T>;

    private:
        static T _orient(const Vec2_t& a, const Vec2_t& b, const Vec2_t& c)
        {
            return (b - a).Cross(c - a);
        }

    public:
        Vec2_t start, end;
        // MAYBE cache this? idk i mean its possible, but idk how useful it will be
        Vec2_t GetDirection() const
        {
            return end - start;
        }
        // Note that both of these extend the segment to a line
        T GetDistToPoint(const Vec2_t &p) const
        {
            Vec2_t dir = GetDirection().GetNormalized();

            Vec2_t point_vec = p - start;
            T dot = dir * point_vec;
            return std::sqrt(point_vec.GetLengthSqr() - dot * dot);
        }
        // Yes there are redundant calcs, no I dont care
        // Returns fraction traced before move vec hits point
        double TraceLineSegmentToPointFrac(const Vec2_t &move_vec, const Vec2_t &point) const
        {
            Vec2_t line_dir = GetDirection().GetNormalized();
            // T parallel_dist = line_dir * move_vec;
            T p_perp_dist = GetDistToPoint(point);
            T dot = line_dir * move_vec;

            T perp_move_distance = std::sqrt(move_vec.GetLengthSqr() - dot * dot);

            // yea idgaf at this point
            return ((double)p_perp_dist / (double)perp_move_distance);
        }
        // Credits: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
        std::optional<Vec2_t> GetIntersect(const _Line &other) const
        {
            T s1_x, s1_y, s2_x, s2_y;
            s1_x = end.x - start.x;
            s1_y = end.y - start.y;
            s2_x = other.end.x - other.start.x;
            s2_y = other.end.y - other.start.y;

            T s = (-s1_y * (start.x - other.start.x) + s1_x * (start.y - other.start.y)) / (-s2_x * s1_y + s1_x * s2_y);
            T t = (s2_x * (start.y - other.start.y) - s2_y * (start.x - other.start.x)) / (-s2_x * s1_y + s1_x * s2_y);

            if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
            {

                return Vec2_t(start.x + (t * s1_x), start.y + (t * s1_y));
            }

            return {}; // No collision
        }
        bool IsInLineTrajectory(const Vec2_t &move_vec, const Vec2_t &point) const
        {
            Vec2_t sd = point - start, ed = point - end;
            return move_vec.Cross(sd) * move_vec.Cross(ed) < 0;
        }

        bool Intersects(const _Line &other) const
        {
            T oa = _orient(other.start, other.end, start),
                ob = _orient(other.start, other.end, end),
                oc = _orient(start, end, other.start),
                od = _orient(start, end, other.end);
            // Proper intersection exists iff opposite signs
            return (oa * ob < 0 && oc * od < 0);
        }
        void Translate(const Vec2_t &vec)
        {
            start += vec;
            end += vec;
        }
    };

    using Line = _Line<num_t>;

}
