#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "utility.hpp"
#include "hittable.hpp"

class moving_sphere : public hittable {
    public:
        point3d cen;
        vec3d velocity;
        double radius;
        shared_ptr<material> mat_ptr;

        moving_sphere() {}
        moving_sphere(
            point3d cen0, point3d cen1, double time0, double time1, double r, shared_ptr<material> m)
            : cen{ cen0 }, velocity{ (cen1 - cen0)/(time1 - time0) }, radius{ r }, mat_ptr{ m } 
        {}
        moving_sphere(point3d cen, vec3d vel, double r, shared_ptr<material> m)
            : cen{ cen }, velocity{ vel }, radius{ r }, mat_ptr{ m } {}

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        point3d center(double time) const;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            aabb b0(
                center(time0) - vec3d(radius),
                center(time0) + vec3d(radius)
            );
            aabb b1(
                center(time1) - vec3d(radius),
                center(time1) + vec3d(radius)
            );
            output_box = surrounding_box(b0, b1);
            return true;
        }
};

point3d moving_sphere::center(double time) const {
    return cen + time * velocity;
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    point3d center = this->center(r.ray_time());
    vec3d oc = r.orig - center;
    double a = r.dir.norm_squared();
    double half_b = dot(oc, r.dir);
    double c = oc.norm_squared() - this->radius * this->radius;

    double discrim = half_b * half_b - a * c;
    if (discrim < 0.0) return false;

    double sqrtd = sqrt(discrim);

    // find the closest root inside our range
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;

        if (root < t_min || root > t_max) 
            return false;
    }

    //if we reach here, we found a root

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3d normal = (rec.p - center) / radius;
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif