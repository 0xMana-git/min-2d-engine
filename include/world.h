#include "math/polygon.h"
#include "misc/typedefs.h"

#include <unordered_map>


namespace Engine {
    //I cant think of way to make this fit every usecase without vtable bloat, so modify if u want
    class World {
        
        std::unordered_map<obj_uid_t id, PolygonBase*> objects_table;
        std::unordered_map<obj_uid_t id, TrianglePolygon> triangles_table;
        std::unordered_map<obj_uid_t id, QuadConvexPolygon> quads_table;
        obj_id_t current_id = 0;
        Vec2 TraceRay(Vec2 start, Vec2 end) {

        }

        //Yes its retarded, No I dont give a fuck
        void MoveTriangle(obj_id_t object_uid, const Vec2& vec) {
            //errors when it doesnt find the right one, cry about it

        }
        void MoveQuad(obj_id_t object_uid, const Vec2& vec) {
        }
        

        template<
        




    };
}