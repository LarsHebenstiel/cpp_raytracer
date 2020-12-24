#include<iostream>
#include<thread>

#include "utility.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "timing.hpp"
#include "threading.hpp"

using std::cout;
using std::cin;
using std::endl;

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3d(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3d center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3d(4, 0.2, 0)).norm() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3d(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3d(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3d(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list caustic_demo() {
    hittable_list world;
    
    auto ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3d(0,-1000,0), 1000, ground));

    auto glass = make_shared<dielectric>(color(1.0,1.0,1.0),1.5);
    world.add(make_shared<sphere>(point3d(0,1,0),0.5,glass));

    auto light = make_shared<diffuse_light>(color(8.0));
    world.add(make_shared<sphere>(point3d(0,5,0),1,light));

    return world;
    }

int main() {
    // Image properties
    const double aspect_ratio = 2.0 / 3.0;
    const int image_width = 200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    const int MSAA_samples_per_pixel = 4;
    const int MC_samples_per_pixel = 1024;

    const int MSAA_subpixel_width = static_cast<int>(sqrt(MSAA_samples_per_pixel));
    const int max_depth = 20;

    const double MSAA_subpixel_size = 1.0 / static_cast<double>(MSAA_subpixel_width);
    
    // World
    hittable_list world = caustic_demo();

    // Camera
    point3d lookfrom(0,2,3);
    point3d lookat(0,0.75,0);
    vec3d vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;

    camera cam(lookfrom, lookat, vup, 35.0, aspect_ratio, aperture, dist_to_focus);

    Timer t;
    t.start();

    // Render

    color * pixels = new color[image_width * image_height];
    const int pixel_block_size = 30;
    std::queue<int *> q = buildPixelBlocks(image_width, image_height, pixel_block_size, pixel_block_size);

    const int num_of_threads = 2;
    std::future<void> thread_futures [num_of_threads];
    for(int i = 0; i < num_of_threads; i++) {
        thread_futures[i] = std::async(std::launch::async, thread_render, 
            std::ref(q), std::ref(pixels), image_width, image_height, std::ref(world),
            std::ref(cam),MSAA_samples_per_pixel,MC_samples_per_pixel,
            MSAA_subpixel_width, MSAA_subpixel_size, max_depth);
    }
    
    std::cerr << num_of_threads << " Threads started, awaiting completion" << endl;
    
    while(q.size() > 0) {
        std::cerr << "\rPixel blocks remaining: " << q.size() << "    " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cerr << "\rPixel blocks remaining: " << 0 << "    " << std::flush;
    
    //make sure all threads are done
    for(auto& f : thread_futures) f.get();

    long long timeMicro = t.elapsedMicro();
    long long timeMilli = timeMicro / 1000;
    std::cerr << "\nDone calculating.\n";
    std::cerr << "Ray tracing took " << timeMilli <<  " milliseconds" << endl;
    std::cerr << "Ray tracing averaged " <<
        static_cast<double>(static_cast<long>(image_width) * static_cast<long>(image_height) * static_cast<long>(MSAA_samples_per_pixel) * static_cast<long>(MC_samples_per_pixel)) / timeMicro 
        <<  " pixel calculations per microsecond" << endl;
    std::cerr << "Writing data to image now." << endl;

    write_image(std::cout, pixels, image_width, image_height, MSAA_samples_per_pixel, MC_samples_per_pixel);

    delete[] pixels;

    return 0;
}