#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "utility.hpp"
#include "hittable.hpp"

class moving_sphere : public hittable {
    public:
        point3 cen;
        vec3 velocity;
        Float radius;
        shared_ptr<material> mat_ptr;

        moving_sphere() {}
        moving_sphere(
            point3 cen0, point3 cen1, Float time0, Float time1, Float r, shared_ptr<material> m)
            : cen{ cen0 }, velocity{ (cen1 - cen0)/(time1 - time0) }, radius{ r }, mat_ptr{ m } 
        {}
        moving_sphere(point3 cen, vec3 vel, Float r, shared_ptr<material> m)
            : cen{ cen }, velocity{ vel }, radius{ r }, mat_ptr{ m } {}

        virtual bool hit(const ray& r, Float t_min, Float t_max, hit_record& rec) const override;

        point3 center(Float time) const;

        virtual bool bounding_box(Float time0, Float time1, aabb& output_box) const override {
            aabb b0(
                center(time0) - vec3(radius),
                center(time0) + vec3(radius)
            );
            aabb b1(
                center(time1) - vec3(radius),
                center(time1) + vec3(radius)
            );
            output_box = surrounding_box(b0, b1);
            return true;
        }
};

point3 moving_sphere::center(Float time) const {
    return cen + time * velocity;
}

bool moving_sphere::hit(const ray& r, Float t_min, Float t_max, hit_record& rec) const {
    point3 center = this->center(r.ray_time());
    vec3 oc = r.orig - center;
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

#endif