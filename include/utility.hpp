#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <chrono>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Functions

inline double degrees_to_radians(double degrees) { return degrees * pi / 180.0; }

//https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers
//creating a new distribution each time is more efficient than 
//using a static [0,1) distribution and adjusting the range
inline double random_double(double min, double max) {
    static thread_local std::mt19937 gen = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count() + static_cast<long long>(std::hash<std::thread::id>()(std::this_thread::get_id())));

    std::uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}

inline double random_double(double max) {
    return random_double(0.0, max);
}

inline double random_double() {
    return random_double(0.0, 1.0);
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common headers
#include "ray.hpp"
#include "vec3d.hpp"

#endif