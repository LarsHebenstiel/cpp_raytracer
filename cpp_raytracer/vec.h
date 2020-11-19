#ifndef VEC_H
#define VEC_H

class vec3 {
    public:
        double x;
        double y;
        double z;
        vec3();
        vec3(double x, double y, double z);
        double norm();
        void normalize();
};

#endif