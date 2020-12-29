#ifndef VEC_H
#define VEC_H

#include<cmath>
#include<iostream>

class vec3 {
    public:
        Float x;
        Float y;
        Float z;

        //size of vector
        const static int n = 3;

        //default constructor
        vec3() : x{ 0 }, y{ 0 }, z{ 0 } {}
        //constructor: initializes to constant array with Float e
        vec3(Float e) : x{ e }, y{ e }, z{ e } {}
        //constructor: intializes to given values
        vec3(Float x, Float y, Float z) : x{ x }, y{ y }, z{ z } {}
        //copy constructor
        vec3(const vec3& u) : x{ u.x }, y{ u.y }, z{ u.z } {}
        //destructor
        ~vec3() {}

        //copy assignment to vector
        void operator=(const vec3& v) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
        }
        
        //copy assignment to vector
        void operator=(const Float v[3]) {
            this->x = v[0];
            this->y = v[1];
            this->z = v[2];
        }

        //vector equality
        bool operator==(const vec3& v) const {
            return this->x == v.x && this->y == v.y && this->z == v.z;
        }
        //vector inequality
        bool operator!=(const vec3& v) const {
            return !(*this == v);
        }

        //vector negation
        vec3 operator-() const {
            return vec3(-this->x, -this->y, -this->z);
        }

        //in place vector addition
        void operator+=(const vec3& v) {
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
        }

        //in place vector subtraction
        void operator-=(const vec3& v) {
            this->x -= v.x;
            this->y -= v.y;
            this->z -= v.z;
        }

        //in place vector scalar multiplication
        void operator*=(Float s) {
            this->x *= s;
            this->y *= s;
            this->z *= s;
        }

        //in place vector scalar division
        void operator/=(Float s) {
            this->x /= s;
            this->y /= s;
            this->z /= s;
        }

        inline Float norm() const;

        inline Float norm_squared() const;

        inline bool near_zero() const;
};

using point3 = vec3; //3D point
using color = vec3; //RGB color

// Utility functions
inline Float dot(const vec3 &u, const vec3 &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline Float vec3::norm() const {
    return sqrt(dot(*this, *this));
}

inline Float vec3::norm_squared() const {
    return dot(*this, *this);
}

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << '(' << v.x << ' ' << v.y << ' ' << v.z << ')';
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(Float t, const vec3 &v) {
    return vec3(t * v.x, t * v.y, t * v.z);
}

inline vec3 operator*(const vec3 &v, Float t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, Float t) {
    return (1/t) * v;
}

inline vec3 operator/(const vec3& v, const vec3& u) {
    return vec3(v.x / u.x, v.y / u.y, v.z / u.z);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.norm();
}

inline vec3 random_vec() {
    return vec3(random_Float(), random_Float(), random_Float());
}

inline vec3 random_vec(Float min, Float max) {
    return vec3(random_Float(min, max), random_Float(min, max), random_Float(min, max));
}

inline bool vec3::near_zero() const {
    // Return true if the vector is close to zero in all dimensions.
    static const Float s = 1e-5;
    return (this->x < s) && (this->y < s) && (this->z < s);
}

vec3 random_in_unit_sphere() {
    while(true) {
        vec3 v = random_vec(-1,1);
        if (v.norm_squared() >= 1) continue;
        return v;
    }
}

vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}
vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    // in the same hemisphere as the normal
    if (dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

vec3 random_in_unit_disk() {
    while (true) {
        vec3 p = vec3(random_Float(-1,1),random_Float(-1,1),0);
        if (p.norm_squared() >= 1) continue;
        return p;
    }
}

// https://blogs.sas.com/content/iml/2020/10/19/random-points-in-triangle.html
vec3 random_in_triangle(point3& p0, point3& p1, point3& p2) {
    vec3 a = p1 - p0;
    vec3 b = p2 - p0;
    Float u = random_Float(0.0, 1.0);
    Float v = random_Float(0.0, 1.0);
    if (u + v > 1.0) {
        u = 1 - u;
        v = 1 - v;
    }
    return p0 + u * a + b * v;
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n;
}

vec3 refract(const vec3& in, const vec3& n, Float etai_over_etat) {
    auto cos_theta = fmin(dot(-in, n), 1.0);
    vec3 r_out_parallel =  etai_over_etat * (in + cos_theta*n);
    vec3 r_out_perp = -sqrt(fabs(1.0 - r_out_parallel.norm_squared())) * n;
    return r_out_perp + r_out_parallel;
}

bool refract(vec3& refracted, const vec3& in, const vec3& n, Float etai_over_etat) {
    auto cos_theta = fmin(dot(-in, n), 1.0);
    if (etai_over_etat * sqrt(1 - cos_theta * cos_theta) > 1.0)
        //total internal reflection
        return false;
    vec3 r_out_parallel =  etai_over_etat * (in + cos_theta*n);
    vec3 r_out_perp = -sqrt(fabs(1.0 - r_out_parallel.norm_squared())) * n;
    refracted = r_out_perp + r_out_parallel;
    return true;
}

#endif