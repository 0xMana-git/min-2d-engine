#include "math/vector.h"
#include "misc/typedefs.h"

#include <span>
namespace Engine {

    template<typename T>
    struct _AxisAlignedRect { 
        using Vec2_t = _Vec2<T>;
        Vec2_t min, max;
        _AxisAlignedRect() {
            min = Vec2_t();
            max = Vec2_t();
        }
        _AxisAlignedRect(const Vec2_t& min, const Vec2_t& max) {
            this->min = min;
            this->max = max;
        }
        _AxisAlignedRect(T minx, T miny, T maxx, T maxy){
            min = Vec2_t(minx, miny);
            max = Vec2_t(maxx, maxy);
        }
        _AxisAlignedRect(const std::span<Vec2_t> verts) {
            double minx = 0, miny = 0, maxx = 0, maxy = 0;
            for(const Vec2d& vert : verts) {
                minx = std::min(minx, vert.x);
                maxx = std::max(minx, vert.x);
                miny = std::min(miny, vert.y);
                maxy = std::max(miny, vert.y);
            }
            min = Vec2_t(minx, miny);
            max = Vec2_t(maxx, maxy);

        }
        bool Intersects(const _AxisAlignedRect& other) const {
            if(min.x > other.max.x || max.x < other.min.x)
                return false;
            if(min.y > other.max.y || max.y < other.min.y)
                return false;
            return true;
        };
        void Translate(const Vec2_t& vec) {
            min += vec;
            max += vec;
        }

    };
    using AxisAlignedRect = _AxisAlignedRect<num_t>;
}