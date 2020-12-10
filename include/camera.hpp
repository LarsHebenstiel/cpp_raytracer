#ifndef CAMERA_H
#define CAMERA_H

#include "utility.hpp"

class camera {
    public:
        point3d origin;
        point3d lower_left_corner;
        vec3d horizontal;
        vec3d vertical;

        camera() {
            double aspect_ratio = 4.0 / 3.0;
            double viewport_height = 2.0;
            double viewport_width = aspect_ratio * viewport_height;
            double focal_length = 1.0;

            this->origin = point3d(0, 0, 0);
            this->horizontal = vec3d(viewport_width, 0.0, 0.0);
            this->vertical = vec3d(0.0, viewport_height, 0.0);
            this->lower_left_corner = origin - horizontal/2 - vertical/2 - vec3d(0, 0, focal_length);
        }

        ray get_ray(double u, double v) const {
            return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
        }
};

#endif