#ifndef VEC_H
#define VEC_H

class vec3 {
    public:
        double x;
        double y;
        double z;
        vec3(double x, double y, double z);
        vec3();
        double norm();
};

#endif