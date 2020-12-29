#ifndef SPHERE_H
#define SPHERE_H

#include<cmath>

#include "utility.hpp"
#include "hittable.hpp"

class sphere : public hittable {
    public:
        point3 center;
        Float radius;
        shared_ptr<material> mat_ptr;

        sphere() {}
        sphere(point3 cen, Float r, shared_ptr<material> m) 
            : center{ cen }, radius { r } , mat_ptr { m } {}

         virtual bool hit(const ray& r, Float t_min, 
            Float t_max, hit_record& rec) const override;

        virtual bool bounding_box(Float time0, Float time1, aabb& output_box) const override;
};

bool sphere::hit(const ray& r, Float t_min, Float t_max, hit_record& rec) const {
    vec3 oc = r.orig - this->center;
    Float a = r.dir.norm_squared();
    Float half_b = dot(oc, r.dir);
    Float c = oc.norm_squared() - this->radius * this->radius;

    Float discrim = half_b * half_b - a * c;
    if (discrim < 0.0) return false;

    Float sqrtd = sqrt(discrim);

    // find the closest root inside our range
    Float root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;

        if (root < t_min || root > t_max) 
            return false;
    }

    //if we reach here, we found a root

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 normal = (rec.p - center) / radius;
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool sphere::bounding_box(Float time0, Float time1, aabb& output_box) const {
    output_box = aabb(
        center - vec3(radius),
        center + vec3(radius)
    );
    
    return true;
}

#endif