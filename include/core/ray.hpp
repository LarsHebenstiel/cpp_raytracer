#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

class ray {
    public:
        point3 orig;
        vec3 dir;
        Float time;

        ray() {}
        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction), time{ 0 } {}
        ray(const point3& origin, const vec3& direction, Float time) : orig(origin), dir(direction), time{ time } {}

        point3 origin() const {return this->orig;}
        vec3 direction() const {return this->dir;}
        Float ray_time() const {return this->time;}

        point3 at(const Float t) const {return orig + t * dir;}
};

#endif //RAY_H