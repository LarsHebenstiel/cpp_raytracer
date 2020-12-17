#include<iostream>

#include "utility.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"

using std::cout;
using std::cin;
using std::endl;

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0)
        return color(0,0,0);

    // min time is 0.0001 to get rid of shadow acne
    if (world.hit(r, 0.0001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }

    vec3d unit_dir = unit_vector(r.dir);
    double t = 0.5 * (unit_dir.y + 1);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    // Image properties
    const double aspect_ratio = 4.0 / 3.0;
    const int image_width = 300;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int MSAA_samples_per_pixel = 1;
    const int MC_samples_per_pixel = 16;
    const int MSAA_subpixel_width = static_cast<int>(sqrt(MSAA_samples_per_pixel));
    const int max_depth = 20;

    assert(MSAA_subpixel_width * MSAA_subpixel_width == MSAA_samples_per_pixel);

    const double MSAA_subpixel_size = 1.0 / static_cast<double>(MSAA_subpixel_width);
    
    // World

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // auto material_left   = make_shared<dielectric>(1.5);
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    // world.add(make_shared<sphere>(point3d( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3d( 0.0,    0.0, -1.0),   0.5, material_center));
    // world.add(make_shared<sphere>(point3d(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<sphere>(point3d(-1.0,    0.0, -1.0), -0.45, material_left));
    // world.add(make_shared<sphere>(point3d( 1.0,    0.0, -1.0),   0.5, material_right));
    world.add(make_shared<sphere>(point3d( 0.0, -1000.5, -1.0), 1000.0, material_ground));
    // world.add(make_shared<sphere>(point3d( 0.0,    0.0, 1.0),   0.5, material_center));
    // world.add(make_shared<sphere>(point3d(-0.5,    0.0, -1.0),   0.5, material_left));
    // //world.add(make_shared<sphere>(point3d(-0.5,    0.0, -1.0),  -0.4, material_left));
    // world.add(make_shared<sphere>(point3d( 1.0,    0.0, -1.0),   0.5, material_right));
    
    auto skymat = make_shared<lambertian>(color(0,0.4,0.5));
    world.add(make_shared<sphere>(point3d(0.0, 12.0,10.0),10.0, skymat));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3d(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3d(4, 1, 0), 1.0, material3));

    auto material1 = make_shared<dielectric>(10);
    world.add(make_shared<sphere>(point3d(0, 1, 0), 1.0, material1));

    // Camera
    point3d lookfrom(13,2,3);
    point3d lookat(0,2,0);
    vec3d vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;

    camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus);


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
                    //use stratified sampling + jitter to emulate blue noise for MSAA
                    double u = static_cast<double>(i + MSAA_subpixel_size * (m % MSAA_subpixel_width + random_double(-0.5, 0.5))) / (image_width - 1);
                    double v = static_cast<double>(j + MSAA_subpixel_size * (m / MSAA_subpixel_width + random_double(-0.5, 0.5))) / (image_height - 1);
                
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
        static_cast<double>(static_cast<long>(image_width) * static_cast<long>(image_height) * static_cast<long>(MSAA_samples_per_pixel) * static_cast<long>(MC_samples_per_pixel)) / timeMicro 
        <<  " pixel calculations per microsecond" << endl;

    return 0;
}