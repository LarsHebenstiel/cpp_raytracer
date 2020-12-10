#include<iostream>

#include "utility.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

using std::cout;
using std::cin;
using std::endl;

color ray_color(const ray& ray, const hittable& world) {
    hit_record rec;

    if (world.hit(ray, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3d unit_dir = unit_vector(ray.dir);
    double t = 0.5 * (unit_dir.y + 1);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    // Image properties
    const double aspect_ratio = 4.0 / 3.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3d(0.0,0.0,-1.0),0.5));
    world.add(make_shared<sphere>(point3d(0.0,-100.5,-1.0),100));

    // Camera properties
    double view_height = 2.0;
    double view_width = view_height * aspect_ratio;
    double focal_length = 1.0;

    point3d origin(0,0,0);
    vec3d horizontal(view_width, 0, 0);
    vec3d vertical(0, view_height, 0);
    vec3d lower_left_corner = origin - horizontal/2 - vertical/2 - vec3d(0, 0, focal_length);

    Timer t;
    t.start();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            double u = static_cast<double>(i) / (image_width - 1);
            double v = static_cast<double>(j) / (image_height - 1);

            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r, world);
            
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
    std::cerr << "Ray tracing took " << t.elapsedMilli() <<  " milliseconds" << endl;

    return 0;
}