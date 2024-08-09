#pragma once

#include "math/polygon.h"
#include "misc/typedefs.h"

#include <unordered_map>


namespace Engine {
    //I cant think of way to make this fit every usecase without vtable bloat, so modify if u want
    class World {
        
        std::unordered_map<obj_id_t, PolygonBase*> objects_table;
        std::unordered_map<obj_id_t, TrianglePolygon> triangles_table;
        std::unordered_map<obj_id_t, QuadConvexPolygon> quads_table;
        obj_id_t current_id = 0;
        // Vec2 TraceRay(const Vec2& start, const Vec2& end) {

        // }
        // Vec2 TraceLine(const Line& line, const Vec2& vec) {

        // }

        //Either returns the object that it collided with, or nothing
        std::optional<obj_id_t> MoveObjectAndCollide(obj_id_t object_uid, const Vec2& vec) {
            PolygonBase& object = *objects_table[object_uid];
            double min_frac = 1.0;
            obj_id_t collided_object_id = 0;
            bool collided = false;
            for(const Line& seg : std::span(object._seg_begin, object._seg_end)){
                for(auto it : objects_table) {
                    if(it.first == object_uid)
                        continue;
                    for(const Vec2& vert : std::span(it.second->_vert_begin, it.second->_vert_end)){
                        if(!seg.IsInLineTrajectory(vec, vert))
                            continue;
                        double frac = seg.TraceLineSegmentToPointFrac(vec, vert);
                        if(!frac < min_frac)
                            continue;
                        collided_object_id = it.first;
                        collided = true;
                    }
                }
            }
            if(collided)
                return collided_object_id;
            return {};
        };



    };
}