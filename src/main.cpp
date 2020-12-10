#include<iostream>

#include "utility.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

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
    const int samples_per_pixel = 16;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3d(0.0,0.0,-1.0),0.5));
    world.add(make_shared<sphere>(point3d(0.0,-100.5,-1.0),100));

    // Camera
    camera cam;

    Timer t;
    t.start();

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            
            for (int s = 0; s < samples_per_pixel; s++) {
                double u = static_cast<double>(i + random_double()) / (image_width - 1);
                double v = static_cast<double>(j + random_double()) / (image_height - 1);

                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    std::cerr << "Ray tracing took " << t.elapsedMilli() <<  " milliseconds" << endl;

    return 0;
}