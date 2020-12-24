#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <queue>
#include "utility.hpp"
#include "hittable.hpp"
#include "material.hpp"

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

std::queue<int*> buildPixelBlocks(int image_width, int image_height, 
        int horizontal_pixel_blocks, int vertical_pixel_blocks) {
    std::queue<int*> q;
    int *width_indexes = new int[horizontal_pixel_blocks + 1];
    int *height_indexes = new int[vertical_pixel_blocks + 1];

    for (int i = 0; i < horizontal_pixel_blocks + 1; i++) {
        width_indexes[i] = i * image_width / horizontal_pixel_blocks;
    } 

    for (int j = 0; j < vertical_pixel_blocks + 1; j++) {
        height_indexes[j] = j * image_height / vertical_pixel_blocks;
    }

    for (int i = 0; i < horizontal_pixel_blocks; i++) {
        for (int j = 0; j < vertical_pixel_blocks; j++) {
            int *arr = new int[4] {width_indexes[i],width_indexes[i + 1],height_indexes[j],height_indexes[j + 1]};
            q.push(arr);
        }
    }
    return q;
}

void write_image(std::ostream& out, color *pixels, int image_width, int image_height,
        int MSAA_samples_per_pixel, int MC_samples_per_pixel) {
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; j--) {
        for (int i = 0; i < image_width; i++) {
            write_color(std::cout, pixels[j * image_width + i], MSAA_samples_per_pixel, MC_samples_per_pixel);
        }
    }

}

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0)
        return color(0,0,0);

    // min time is 0.0001 to get rid of shadow acne
    if (world.hit(r, 0.0001, infinity, rec)) {
        ray scattered;
        color attenuation;
        color emitted = rec.mat_ptr->emitted();

        if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return emitted;
            
        return emitted + attenuation * ray_color(scattered, world, depth-1);
    }

    vec3d unit_dir = unit_vector(r.dir);
    double t = 0.5 * (unit_dir.y + 1);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

#endif