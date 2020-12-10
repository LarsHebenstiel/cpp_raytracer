#include<iostream>
#include<cmath>
#include<chrono>

#include "vec3d.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "timing.hpp"

using std::cout;
using std::cin;
using std::endl;

double hit_sphere(const point3d& center, double radius, const ray& r) {
    vec3d oc = r.orig - center;
    double a = r.dir.norm_squared();
    double half_b = dot(oc,r.dir);
    double c = oc.norm_squared() - radius * radius;
    double discrim = half_b * half_b - a * c;
    if (discrim < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discrim)) / a;
    }
}



color ray_color(const ray& ray) {
    double t = hit_sphere(point3d(0.3,-0.4,-1),0.5,ray);
    if (t > 0.0) {
        vec3d n = unit_vector(ray.at(t) - point3d(0,0,-1));
        return 0.5 * color(n.x + 1, n.y + 1, n.z + 1);
    }
    vec3d unit_dir = unit_vector(ray.dir);
    t = 0.5 * (unit_dir.y + 1);
    return (1.0-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    // Image properties
    const double aspect_ratio = 4.0 / 3.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

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
            color pixel_color = ray_color(r);
            
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
    std::cerr << "Ray tracing took " << t.elapsedMilli() <<  " milliseconds" << endl;

    return 0;
}