#include "vec.h"
#include <cmath>

vec3::vec3(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3::vec3() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

double vec3::norm() {
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}