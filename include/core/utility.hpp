#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <chrono>
#include <string>

#include "macros.hpp"

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const Float infinity = std::numeric_limits<Float>::infinity();
const Float pi = 3.1415926535897932385;
const Float invpi = 1 / pi;

// Functions

std::vector<std::string> split(const std::string &phrase, const std::string &delim) {
    std::vector<std::string> res;
    std::string s(phrase);
    size_t pos = 0;
    std::string token;

    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        res.push_back(token);
        s.erase(0, pos + delim.length());
    }
    res.push_back(s);

    return res;
}

void writeStringAsHex(std::string &s, std::ostream &os) {
    os << std::hex;

    for (size_t i = 0; i < s.length(); i++) {
        os << static_cast<int>(s[i]) << " ";
    }

    os << "\n"
       << std::dec;
}

int count(std::string &s, char c) {
    int res = 0;

    for (size_t i = 0; i < s.length(); i++)
        if (s[i] == c)
            res++;

    return res;
}

inline Float degrees_to_radians(Float degrees) { return degrees * pi / 180.0; }

//https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers
//creating a new distribution each time is more efficient than 
//using a static [0,1) distribution and adjusting the range
inline Float random_Float(Float min, Float max) {
    static thread_local std::mt19937 gen = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count() + static_cast<long long>(std::hash<std::thread::id>()(std::this_thread::get_id())));

    std::uniform_real_distribution<Float> dist(min, max);
    return dist(gen);
}

inline Float random_Float(Float max) {
    return random_Float(0.0, max);
}

inline Float random_Float() {
    return random_Float(0.0, 1.0);
}

inline Float clamp(Float x, Float min, Float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common headers
#include "ray.hpp"
#include "vec3.hpp"

// More functions that rely on common headers

vec3 clamp(const vec3& v, Float min, Float max) {
    return vec3(clamp(v.x, min, max), clamp(v.y, min, max), clamp(v.z, min, max));
}

#endif //UTILITY_H