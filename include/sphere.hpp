#ifndef SPHERE_H
#define SPHERE_H

#include<cmath>

#include "hittable.hpp"

class sphere : public hittable {
    public:
        point3d center;
        double radius;

        sphere() {}
        sphere(point3d cen, double r) : center{ cen }, radius { r } {}

         virtual bool hit(const ray& r, double t_min, 
            double t_max, hit_record& rec) const override;
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

    return true;
};

#endif