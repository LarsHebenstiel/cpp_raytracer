#ifndef TIMING_H
#define TIMING_H

#include <chrono>

// https://stackoverflow.com/questions/12231166/timing-algorithm-clock-vs-time-in-c
class Timer{
    public:
        std::chrono::steady_clock::time_point t1;

        void start() {
            t1 = std::chrono::high_resolution_clock::now();
        }

        long long elapsedMilli() {
            auto t2 = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        }

        long long elapsedMicro() {
            auto t2 = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        }
};

#endif