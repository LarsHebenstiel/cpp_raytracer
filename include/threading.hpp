//https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers
#if defined (_MSC_VER)  // Visual studio
    #define thread_local __declspec( thread )
#elif defined (__GCC__) // GCC
    #define thread_local __thread
#endif

#include <thread>
#include <future>
#include <queue>
#include "utility.hpp"
#include "hittable.hpp"
#include <mutex>
#include "camera.hpp"

//use from writing to cout from threads
//std::mutex cout_mtx

//use for operating on pixel block queue
std::mutex q_mtx;

template<typename T>
bool future_is_ready(std::future<T>& t) {
    return t.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

void thread_render(std::queue<int *>& q, vec3d * pixels, int image_width, int image_height, 
        const hittable& world, const camera& cam, int MSAA_samples_per_pixel, int MC_samples_per_pixel,
        int MSAA_subpixel_width, double MSAA_subpixel_size, int max_depth) {
    bool cont;
    int * arr;

    //attempt to take an array to process
    q_mtx.lock();
    cont = (q.size() > 0);
    if (cont) {
        arr = q.front();
        q.pop();
    }
    q_mtx.unlock();

    // continue if we got an array to process
    while(cont) {
        //do processing
        for (int j = arr[2]; j < arr[3]; j++) {
            for (int i = arr[0]; i < arr[1]; i++) {
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
                pixels[j * image_width + i] = pixel_color;
            }
        }
        
        //array was new[] allocated, we must delete[]
        delete[] arr;

        //finished processing, look for new array to process
        q_mtx.lock();
        cont = (q.size() > 0);
        if (cont) {
            arr = q.front();
            q.pop();
        }
        q_mtx.unlock();
    }
}