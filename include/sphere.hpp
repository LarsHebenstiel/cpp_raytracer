#ifndef SPHERE_H
#define SPHERE_H

#include<cmath>

#include "hittable.hpp"

class sphere : public hittable {
    public:
        point3d center;
        double radius;
        shared_ptr<material> mat_ptr;

        sphere() {}
        sphere(point3d cen, double r, shared_ptr<material> m) 
            : center{ cen }, radius { r } , mat_ptr { m } {}

         virtual bool hit(const ray& r, double t_min, 
            double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3d oc = r.orig - this->center;
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

bool sphere::bounding_box(double, double, aabb& output_box) const {
    output_box = aabb(
        center - vec3d(radius),
        center + vec3d(radius)
    );
    
    return true;
}

#endif