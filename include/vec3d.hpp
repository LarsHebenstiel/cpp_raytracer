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
            return vec3d(-this->x, -this->y, -this->z);
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

        inline double norm_squared() const;

        inline static vec3d random() {
            return vec3d(random_double(), random_double(), random_double());
        }

        inline static vec3d random(double min, double max) {
            return vec3d(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        inline bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return (this->x < s) && (this->y < s) && (this->z < s);
        }
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

inline double vec3d::norm_squared() const {
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

vec3d random_in_unit_sphere() {
    while(true) {
        vec3d v = vec3d::random(-1,1);
        if (v.norm_squared() >= 1) continue;
        return v;
    }
}

vec3d random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}
vec3d random_in_hemisphere(const vec3d& normal) {
    vec3d in_unit_sphere = random_in_unit_sphere();
    // in the same hemisphere as the normal
    if (dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

vec3d reflect(const vec3d& v, const vec3d& n) {
    return v - 2 * dot(v,n) * n;
}

vec3d refract(const vec3d& in, const vec3d& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-in, n), 1.0);
    vec3d r_out_parallel =  etai_over_etat * (in + cos_theta*n);
    vec3d r_out_perp = -sqrt(fabs(1.0 - r_out_parallel.norm_squared())) * n;
    return r_out_perp + r_out_parallel;
}

bool refract(vec3d& refracted, const vec3d& in, const vec3d& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-in, n), 1.0);
    if (etai_over_etat * sqrt(1 - cos_theta * cos_theta) > 1.0)
        //total internal reflection
        return false;
    vec3d r_out_parallel =  etai_over_etat * (in + cos_theta*n);
    vec3d r_out_perp = -sqrt(fabs(1.0 - r_out_parallel.norm_squared())) * n;
    refracted = r_out_perp + r_out_parallel;
    return true;
}

#endif