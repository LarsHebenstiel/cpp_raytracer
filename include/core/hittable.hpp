#ifndef HITTABLE_H
#define HITTABLE_H

#include "utility.hpp"
#include "aabb.hpp"

class material;

struct hit_record {
    public:
        Float t;
        vec3 normal;
        point3 p;
        bool front_face;
        shared_ptr<material> mat_ptr;

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            // if we are on front face, dot product is negative
            front_face = dot(r.dir, outward_normal) < 0;

            //adjust normal vector accordingly
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() {}
        
        virtual bool hit(const ray& r, Float t_min, 
            Float t_max, hit_record& rec) const = 0;

        virtual bool bounding_box(Float time0, Float time1, aabb& output_box) const = 0;
};

#endif //HITTABLE_H