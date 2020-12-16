#include<iostream>

#include "utility.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

using std::cout;
using std::cin;
using std::endl;

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.0001, infinity, rec)) {
        //use true Lambertian reflection
        point3d target = rec.p + rec.normal + random_unit_vector();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
    }

    vec3d unit_dir = unit_vector(r.dir);
    double t = 0.5 * (unit_dir.y + 1);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    // Image properties
    const double aspect_ratio = 4.0 / 3.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int MSAA_samples_per_pixel = 16;
    const int MC_samples_per_pixel = 16;
    const int MSAA_subpixel_width = static_cast<int>(sqrt(MSAA_samples_per_pixel));
    const int max_depth = 10;

    assert(MSAA_subpixel_width * MSAA_subpixel_width == MSAA_samples_per_pixel);

    const double MSAA_subpixel_size = 1.0 / static_cast<double>(MSAA_subpixel_width);
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
            
            for (int s = 0; s < MC_samples_per_pixel; s++) {
                for (int m = 0; m < MSAA_samples_per_pixel; m++) {
                double u = static_cast<double>(i + MSAA_subpixel_size * (m % MSAA_subpixel_width)) / (image_width - 1);
                double v = static_cast<double>(j + MSAA_subpixel_size * (m / MSAA_subpixel_width)) / (image_height - 1);
                
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
                }
            }
            
            write_color(std::cout, pixel_color, MSAA_samples_per_pixel, MC_samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    long long timeMicro = t.elapsedMicro();
    long long timeMilli = timeMicro / 1000;
    std::cerr << "Ray tracing took " << timeMilli <<  " milliseconds" << endl;
    std::cerr << "Ray tracing averaged " <<
        static_cast<double>(image_width * image_height * MSAA_samples_per_pixel) / timeMicro 
        <<  " pixel color calculations per microsecond" << endl;

    return 0;
}