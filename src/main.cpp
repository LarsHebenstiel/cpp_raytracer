#define USE_FLOAT_AS_DOUBLE

// preprocessor macros
#include "macros.hpp"

#include <iostream>
#include <fstream>
#include <thread>

#include "color.hpp"
#include "camera.hpp"
#include "timing.hpp"
#include "threading.hpp"
#include "bvh_node.hpp"

#include "sample_scenes.hpp"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

int main() {

    std::string filename("/Users/Lars/git/cpp_raytracer/models/geodesic/geodesic_classI_2.obj");
    //std::string filename("/Users/Lars/git/cpp_raytracer/models/bunny.obj");
    std::string logfile("log.log");

    std::ofstream log(logfile);
    
    if (sizeof(Float) == 4) {
        log << "Type Float is using type: float\n\n";
    } else {
        log << "Type Float is using type: double\n\n";
    }
    
    // Image properties
    const Float aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    const int MSAA_samples_per_pixel = 4;
    const int MC_samples_per_pixel = 16;

    const int MSAA_subpixel_width = static_cast<int>(sqrt(MSAA_samples_per_pixel));
    const int max_depth = 20;

    const Float MSAA_subpixel_size = 1.0 / static_cast<Float>(MSAA_subpixel_width);

    // Camera
    point3 lookfrom(4, 0.5, 4);
    point3 lookat(0, 0, 0);
    vec3 vup(0,1,0);
    auto dist_to_focus = (lookfrom - lookat).norm();
    auto aperture = 0;
    Float time0 = 0.0;
    Float time1 = 1.0;

    camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus, time0, time1);

    hittable_list objs = test_obj_file(filename, log);

    log << "[BVH] Starting BVH construction\n" << std::flush;

    auto bvh = make_shared<bvh_node>(objs, time0, time1);

    log << "[/BVH] BVH construction finished\n\n";

    const hittable_list world(bvh);
    //const hittable_list world = objs;

    log << "[Render] Render starting\n";

    Timer t;
    t.start();

    // Render

    log << "\t[Image Blocks]Building image blocks\n" << std::flush;
    color *pixels = new color[image_width * image_height];
    const int pixel_block_size = 30;
    std::queue<int *> q = buildPixelBlocks(image_width, image_height, pixel_block_size, pixel_block_size);
    log << "\t\tImage divided into " << pixel_block_size << "x" << pixel_block_size << " blocks\n";
    log << "\t[/Image Blocks]Finishd building image blocks\n";

    const int num_of_threads = 4;
    std::future<void> thread_futures [num_of_threads];
    log << "\tStarting " << num_of_threads << " threads\n" << std::flush;
    for(int i = 0; i < num_of_threads; i++) {
        thread_futures[i] = std::async(std::launch::async, thread_render, 
            std::ref(q), std::ref(pixels), image_width, image_height, std::ref(world),
            std::ref(cam),MSAA_samples_per_pixel,MC_samples_per_pixel,
            MSAA_subpixel_width, MSAA_subpixel_size, max_depth);
    }
    
    cerr << num_of_threads << " Threads started, awaiting completion" << endl;
    
    while(q.size() > 0) {
        cerr << "\rPixel blocks remaining: " << q.size() << "    " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cerr << "\rPixel blocks remaining: " << 0 << "    " << std::flush;
    
    //make sure all threads are done
    for(auto& f : thread_futures) f.get();

    long long timeMicro = t.elapsedMicro();
    long long timeMilli = timeMicro / 1000;
    cerr << "\nDone calculating.\n";
    cerr << "Ray tracing took " << timeMilli <<  " milliseconds" << endl;
    cerr << "Ray tracing averaged " <<
        static_cast<Float>(static_cast<long>(image_width) * static_cast<long>(image_height) * static_cast<long>(MSAA_samples_per_pixel) * static_cast<long>(MC_samples_per_pixel)) / timeMicro 
        <<  " pixel calculations per microsecond" << endl;

    log << "\tDone calculating\n";
    log << "\tRay tracing took " << timeMilli <<  " milliseconds\n";
    log << "\tRay tracing averaged " <<
        static_cast<Float>(static_cast<long>(image_width) * static_cast<long>(image_height) * static_cast<long>(MSAA_samples_per_pixel) * static_cast<long>(MC_samples_per_pixel)) / timeMicro 
        <<  " pixel calculations per microseconds\n" << std::flush;
    
    log << "\tWriting data to image now\n";
    write_image(std::cout, pixels, image_width, image_height, MSAA_samples_per_pixel, MC_samples_per_pixel);
    log << "\tFinished writing data to image\n" << std::flush;

    delete[] pixels;

    log << "[/Render] Rendering complete";
    log.close();

    return 0;
}