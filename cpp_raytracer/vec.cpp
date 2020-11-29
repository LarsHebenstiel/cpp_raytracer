#include <iostream>
#include <cmath>
#include "vec.h"

using std::cout;
using std::cin;
using std::endl;

vec3d::vec3d() : v{ 0 } {}
vec3d::vec3d(const double e) : v{ e } {}
vec3d::vec3d(const double x, const double y, const double z) : v{ x, y, z } {}
vec3d::vec3d(const vec3d& u) : v{ u.x, u.y, u.z } {}
vec3d::~vec3d() {}

std::ostream& operator<<(std::ostream& os, const vec3d& v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

double vec3d::operator[](const int i) const {return v[i];}
double& vec3d::operator[](const int i) {return v[i];}

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

int main() {
    vec3d u(1,2,3);

    cout << u << endl;
}