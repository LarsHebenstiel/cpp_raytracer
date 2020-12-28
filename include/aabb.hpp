#ifndef AABB_H
#define AABB_H

#include "hittable.hpp"
#include "utility.hpp"

class aabb {
    public:
        point3d min;
        point3d max;

        aabb() {}
        aabb(const point3d& min, const point3d& max) : min{ min }, max{ max } {}

        bool hit(const ray& r, double t_min, double t_max) const {
            double t0 = fmin((min.x - r.orig.x) / r.dir.x, (max.x - r.orig.x) / r.dir.x);
            double t1 = fmax((min.x - r.orig.x) / r.dir.x, (max.x - r.orig.x) / r.dir.x);

            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max? t1 : t_max;

            if (t_max <= t_min) return false;

            t0 = fmin((min.y - r.orig.y) / r.dir.y, (max.y - r.orig.y) / r.dir.y);
            t1 = fmax((min.y - r.orig.y) / r.dir.y, (max.y - r.orig.y) / r.dir.y);

            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max? t1 : t_max;

            if (t_max <= t_min) return false;

            t0 = fmin((min.z - r.orig.z) / r.dir.z, (max.z - r.orig.z) / r.dir.z);
            t1 = fmax((min.z - r.orig.z) / r.dir.z, (max.z - r.orig.z) / r.dir.z);

            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max? t1 : t_max;

            if (t_max <= t_min) return false;

            return true;
        }
};

aabb surrounding_box(aabb& b0, aabb& b1) {
    point3d small(fmin(b0.min.x, b1.min.x),
                  fmin(b0.min.y, b1.min.y),
                  fmin(b0.min.z, b1.min.z));

    point3d big(fmin(b0.max.x, b1.max.x),
                  fmin(b0.max.y, b1.max.y),
                  fmin(b0.max.z, b1.max.z));

    return aabb(small, big);
}

#endif