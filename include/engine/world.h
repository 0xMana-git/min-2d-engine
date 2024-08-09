#pragma once

#include "engine/math/polygon.h"
#include "engine/misc/typedefs.h"

#include <unordered_map>
#include <typeinfo>
#include <cassert>
#include <cmath>
#include <limits>

namespace Engine {
    //I cant think of way to make this fit every usecase without vtable bloat, so modify if u want
    class World {
        
        std::unordered_map<obj_id_t, PolygonBase*> objects_table;
        std::unordered_map<obj_id_t, TrianglePolygon*> triangles_table;
        std::unordered_map<obj_id_t, QuadConvexPolygon*> quads_table;
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
        std::vector<obj_id_t> GetCollisions(const _PolygonType& polygon, bool use_specialized) {
            std::vector<obj_id_t> collided;
            if(use_specialized) {
                for(auto& it : quads_table) {
                    if(polygon.Intersects(*it.second))
                        collided.push_back(it.first);
                }
                for(auto& it : triangles_table) {
                    if(polygon.Intersects(*it.second))
                        collided.push_back(it.first);
                }
            }
            else {
                for(auto& it : objects_table) {
                    if(polygon.Intersects(*it.second))
                        collided.push_back(it.first);
                }
            }
                
            return collided;
        }

        //NOTE: LOTS OF FUCKY SHIT GOING ON HERE, WATCH THE FUCK OUT
        //Either returns the object that it collided with, or nothing
        std::optional<obj_id_t> MoveObjectAndCollide(obj_id_t object_uid, const Vec2& vec) {
            
            //TODO: check for intersections at new position FIRST, then do the scanlines shit ONLY FOR INTERSECTED OBJECTS
            PolygonBase& object_orig = *objects_table[object_uid];
            //just a placeholder value
            PolygonBase& object = object_orig;
            std::vector<obj_id_t> collisions;
            if(typeid(object_orig) == typeid(TrianglePolygon)){
                TrianglePolygon object_new = static_cast<TrianglePolygon&>(object_orig);
                object_new.Translate(vec);
                PolygonBase& object = object_new;
                collisions = GetCollisions(object_new, true);
                
            }
            else if (typeid(object_orig) == typeid(QuadConvexPolygon)) {
                QuadConvexPolygon object_new = static_cast<QuadConvexPolygon&>(object_orig);
                object_new.Translate(vec);
                PolygonBase& object = object_new;
                collisions = GetCollisions(object_new, true);
            }
            else {
                //i dunno dude
                collisions = GetCollisions(object, false);
                throw std::exception();
            }

            obj_id_t collided_object_id = 0;

            
            if(collisions.size() == 0)
                return {};

            double frac = std::numeric_limits<double>::max();
            for(obj_id_t obj_id : collisions) {
                double res = object.TracePolygon(*objects_table[obj_id], vec);
                if(res < frac) {
                    frac = res;
                    collided_object_id = obj_id;
                }
            }
            assert(frac < 1);
            
            
            return collided_object_id;
        };
        //Please dont call this...
        void AddObject(PolygonBase* obj, obj_id_t idx) {
            if(typeid(obj) == typeid(TrianglePolygon*))
                triangles_table[idx] = static_cast<TrianglePolygon*>(obj);
            else if(typeid(obj) == typeid(QuadConvexPolygon*))
                quads_table[idx] = static_cast<QuadConvexPolygon*>(obj);
            objects_table[idx] = obj;
        }
        void AddObject(TrianglePolygon* obj, obj_id_t idx) {
            triangles_table[idx] = obj;
            objects_table[idx] = obj;
        }
        void AddObject(QuadConvexPolygon* obj, obj_id_t idx) {

            quads_table[idx] = obj;
            objects_table[idx] = obj;
        }



    };
}