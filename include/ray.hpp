#ifndef RAY_H
#define RAY_H

#include "vec3d.hpp"

class ray {
    public:
        point3d orig;
        vec3d dir;

        ray() {}
        ray(const point3d& origin, const vec3d& direction) : orig(origin), dir(direction) {}

        point3d origin() const {return this->orig;}
        vec3d direction() const {return this->dir;}

        point3d at(const double t) const {return orig + t * dir;}
};

#endif