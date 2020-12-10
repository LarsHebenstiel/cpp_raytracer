#ifndef VEC_H
#define VEC_H

#include<cmath>
#include<iostream>

class vec3d {
    public:
        double x;
        double y;
        double z;

        //size of vector
        const static int n = 3;

        //default constructor
        vec3d() : x{ 0 }, y{ 0 }, z{ 0 } {}
        //constructor: initializes to constant array with double e
        vec3d(double e) : x{ e }, y{ e }, z{ e } {}
        //constructor: intializes to given values
        vec3d(double x, double y, double z) : x{ x }, y{ y }, z{ z } {}
        //copy constructor
        vec3d(const vec3d& u) : x{ u.x }, y{ u.y }, z{ u.z } {}
        //destructor
        ~vec3d() {}

        //copy assignment to vector
        void operator=(const vec3d& v) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
        }
        
        //copy assignment to vector
        void operator=(const double v[3]) {
            this->x = v[0];
            this->y = v[1];
            this->z = v[2];
        }

        //vector equality
        bool operator==(const vec3d& v) const {
            return this->x == v.x && this->y == v.y && this->z == v.z;
        }
        //vector inequality
        bool operator!=(const vec3d& v) const {
            return !(*this == v);
        }

        //vector negation
        vec3d operator-() const {
            return vec3d(this->x, this->y, this->z);
        }

        //in place vector addition
        void operator+=(const vec3d& v) {
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
        }

        //in place vector subtraction
        void operator-=(const vec3d& v) {
            this->x -= v.x;
            this->y -= v.y;
            this->z -= v.z;
        }

        //in place vector scalar multiplication
        void operator*=(double s) {
            this->x *= s;
            this->y *= s;
            this->z *= s;
        }

        //in place vector scalar division
        void operator/=(double s) {
            this->x /= s;
            this->y /= s;
            this->z /= s;
        }

        inline double norm() const;

        inline double squared_norm() const;
};

using point3d = vec3d; //3D point
using color = vec3d; //RGB color

// Utility functions
inline double dot(const vec3d &u, const vec3d &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline vec3d cross(const vec3d &u, const vec3d &v) {
    return vec3d(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline double vec3d::norm() const {
    return sqrt(dot(*this, *this));
}

inline double vec3d::squared_norm() const {
    return dot(*this, *this);
}

inline std::ostream& operator<<(std::ostream &out, const vec3d &v) {
    return out << '(' << v.x << ' ' << v.y << ' ' << v.z << ')';
}

inline vec3d operator+(const vec3d &u, const vec3d &v) {
    return vec3d(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3d operator-(const vec3d &u, const vec3d &v) {
    return vec3d(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3d operator*(const vec3d &u, const vec3d &v) {
    return vec3d(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3d operator*(double t, const vec3d &v) {
    return vec3d(t * v.x, t * v.y, t * v.z);
}

inline vec3d operator*(const vec3d &v, double t) {
    return t * v;
}

inline vec3d operator/(vec3d v, double t) {
    return (1/t) * v;
}

inline vec3d unit_vector(const vec3d& v) {
    return v / v.norm();
}

#endif