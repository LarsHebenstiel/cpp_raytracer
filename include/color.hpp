#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3d.hpp"

void write_color(std::ostream &out, const color& pixel_color, 
                    int MSAA_samples_per_pixel, int MC_samples_per_pixel) {
    double scale = 1.0 / static_cast<double>(MSAA_samples_per_pixel * MC_samples_per_pixel);
    //apply sqrt for gamma correction with gamma = 2;
    double r = sqrt(pixel_color.x * scale);
    double g = sqrt(pixel_color.y * scale);
    double b = sqrt(pixel_color.z * scale);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0, 0.9999)) << ' '
        << static_cast<int>(256 * clamp(g, 0, 0.9999)) << ' '
        << static_cast<int>(256 * clamp(b, 0, 0.9999)) << '\n';
}

#endif