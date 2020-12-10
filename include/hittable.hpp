#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3d.hpp"
#include "ray.hpp"

struct hit_record {
    public:
        double t;
        vec3d normal;
        point3d p;
        bool front_face;

        void set_face_normal(const ray& r, const vec3d& outward_normal) {
            // if we are on front face, dot product is negative
            front_face = dot(r.dir, outward_normal) < 0;

            //adjust normal vector accordingly
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() {}
        
        virtual bool hit(const ray& r, double t_min, 
            double t_max, hit_record& rec) const 
            {return false;}
};

#endif