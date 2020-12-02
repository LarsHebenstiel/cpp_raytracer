#include <iostream>
#include "vec.hpp"

using std::cout;
using std::endl;

vec3d::vec3d() : x{ 0 }, y{ 0 }, z{ 0 } {}
vec3d::vec3d(double e) : x{ e }, y{ e }, z{ e } {}
vec3d::vec3d(double x, double y, double z) : x{ x }, y{ y }, z{ z } {}
vec3d::vec3d(const vec3d& u) : x{ u.x }, y{ u.y }, z{ u.z } {}
vec3d::~vec3d() {}

std::ostream& operator<<(std::ostream& os, const vec3d& v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

void vec3d::operator=(const vec3d& v) {this->x = v.x; this->y = v.y; this->z = v.z;}
void vec3d::operator=(const double v[3]) {this->x = v[0]; this->y = v[1]; this->z = v[2];}

bool vec3d::operator==(const vec3d& v) const {return this->x == v.x && this->y == v.y && this->z == v.z;}
bool vec3d::operator!=(const vec3d& v) const {return !(*this == v);}

vec3d vec3d::operator+(const vec3d& v) const {return vec3d(this->x + v.x, this->y + v.y, this->z + v.z);}
void vec3d::operator+=(const vec3d& v) {this->x += v.x; this->y += v.y; this->z += v.z;}

vec3d vec3d::operator-(const vec3d& v) const {return vec3d(this->x - v.x, this->y - v.y, this->z - v.z);}
void vec3d::operator-=(const vec3d& v) {this->x -= v.x; this->y -= v.y; this->z -= v.z;}

//vector scalar product
vec3d vec3d::operator*(const double s) const {return vec3d(s * this->x, s * this->y, s * this->z);}
//vector scalar division
vec3d vec3d::operator/(const double s) const {return vec3d(this->x / s, this->y / s, this->z / s);}

//vector inner product
double vec3d::operator*(const vec3d& v) const {return this->x * v.x + this->y * v.y + this->z + v.z;}
//vector outer product
vec3d vec3d::operator&(const vec3d& v) const {return vec3d(this->y*v.z - this->z*v.y, this->z*v.x - this->x*v.z, this->x*v.y - this->y*v.x);}
//in place vector outer product
void vec3d::operator&=(const vec3d& v) {*this = *this & v;}