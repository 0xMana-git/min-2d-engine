#pragma once

#include "engine/math/polygon.h"
#include "engine/misc/typedefs.h"

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
        public:
        enum CollideTargets {
            TARGET_ALL,
            TARGET_TRI,
            TARGET_QUAD,
        };
        
        template<typename _PolygonType>
        std::vector<obj_id_t> GetCollisions(const _PolygonType& polygon, CollideTargets targets = TARGET_ALL) {
            std::vector<obj_id_t> collided;
            switch (targets)
            {
            case TARGET_QUAD:
                for(auto& it : quads_table) {
                    if(polygon.Intersects(it.second))
                        collided.push_back(it.first);
                }
                break;
            case TARGET_TRI:
                for(auto& it : triangles_table) {
                    if(polygon.Intersects(it.second))
                        collided.push_back(it.first);
                }
            default:
                for(auto& it : objects_table) {
                    if(polygon.Intersects(it.second))
                        collided.push_back(it.first);
                }
                break;
            }
            return collided;
        }

        //Either returns the object that it collided with, or nothing
        std::optional<obj_id_t> MoveObjectAndCollide(obj_id_t object_uid, const Vec2& vec) {
            PolygonBase& object = *objects_table[object_uid];
            double min_frac = 1.0;
            obj_id_t collided_object_id = 0;
            bool collided = false;
            for(const Line& seg : object.segs_view.GetConst()){
                for(auto it : objects_table) {
                    if(it.first == object_uid)
                        continue;
                    for(const Vec2& vert : it.second->verts_view.GetConst()){
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
        
        obj_id_t AddObject(TrianglePolygon obj) {
            obj_id_t index = current_id;
            current_id++;
            triangles_table[index] = obj;
            objects_table[index] = &triangles_table[index];
            return index;
        }
        obj_id_t AddObject(QuadConvexPolygon obj) {
            obj_id_t index = current_id;
            current_id++;
            quads_table[index] = obj;
            objects_table[index] = &triangles_table[index];
            return index;
        }



    };
}