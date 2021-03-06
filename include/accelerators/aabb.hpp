#ifndef AABB_H
#define AABB_H

#include "hittable.hpp"
#include "utility.hpp"

/*
 * Axis-aligned bounding box
 */
class aabb{
    public:
        point3 min;
        point3 max;

        aabb() {}
        aabb(const point3& min, const point3& max) 
        : min{ point3(fmin(min.x, max.x),fmin(min.y, max.y),fmin(min.z, max.z)) }
        , max{ point3(fmax(min.x, max.x),fmax(min.y, max.y),fmax(min.z, max.z)) } 
        {}

        bool hit(const ray& r, Float t_min, Float t_max) const {
            Float t0 = fmin((min.x - r.orig.x) / r.dir.x, (max.x - r.orig.x) / r.dir.x);
            Float t1 = fmax((min.x - r.orig.x) / r.dir.x, (max.x - r.orig.x) / r.dir.x);

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

aabb surrounding_box(const aabb& b0, const aabb& b1) {
    point3 small(fmin(b0.min.x, b1.min.x),
                  fmin(b0.min.y, b1.min.y),
                  fmin(b0.min.z, b1.min.z));

    point3 big(fmax(b0.max.x, b1.max.x),
                  fmax(b0.max.y, b1.max.y),
                  fmax(b0.max.z, b1.max.z));

    return aabb(small, big);
}

aabb surrounding_box(const aabb& b0, const point3& p) {
    point3 small(fmin(b0.min.x, p.x),
                  fmin(b0.min.y, p.y),
                  fmin(b0.min.z, p.z));

    point3 big(fmax(b0.max.x, p.x),
                  fmax(b0.max.y, p.y),
                  fmax(b0.max.z, p.z));

    return aabb(small, big);
}

#endif //AABB_H