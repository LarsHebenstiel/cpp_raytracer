#include<iostream>
#include<thread>

#include "color.hpp"
#include "camera.hpp"
#include "timing.hpp"
#include "threading.hpp"
#include "bvh_node.hpp"

#include "sample_scenes.hpp"

using std::cout;
using std::cin;
using std::endl;

int main() {
    // Image properties
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    const int MSAA_samples_per_pixel = 4;
    const int MC_samples_per_pixel = 512;

    const int MSAA_subpixel_width = static_cast<int>(sqrt(MSAA_samples_per_pixel));
    const int max_depth = 20;

    const double MSAA_subpixel_size = 1.0 / static_cast<double>(MSAA_subpixel_width);

    // Camera
    point3d lookfrom(7,2,7);
    point3d lookat(0,1,0);
    vec3d vup(0,1,0);
    auto dist_to_focus = (lookfrom - lookat).norm();
    auto aperture = 0.1;
    double time0 = 0.0;
    double time1 = 1.0;

    camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus, time0, time1);
        
    // World constructed with BVH
    const hittable_list world(make_shared<bvh_node>(lights_scene(), time0, time1));

    Timer t;
    t.start();

    // Render

    color *pixels = new color[image_width * image_height];
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