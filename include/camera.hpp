#ifndef CAMERA_H
#define CAMERA_H

#include "utility.hpp"

class camera {
    public:
        point3d origin;
        point3d lower_left_corner;
        vec3d horizontal;
        vec3d vertical;
        vec3d forward, right, up;
        double lens_radius;
        double time0, time1; //open close times for the shutter

        camera(
            point3d lookfrom,
            point3d lookat,
            vec3d vup,
            double vert_fov,
            double aspect_ratio,
            double aperture,
            double focal_distance,
            double time0,
            double time1
        ) {
            double theta = degrees_to_radians(vert_fov);
            double h = tan(theta / 2.0);
            double view_height = 2.0 * h;
            double view_width = aspect_ratio * view_height;
            
            //use specified camera position to get the named vectors
            this->forward = unit_vector(lookat - lookfrom);
            this->right = unit_vector(cross(forward, vup));
            this->up = cross(right, forward);

            this->origin = lookfrom;
            this->horizontal = focal_distance * view_width * right;
            this->vertical = focal_distance * view_height * up;
            this->lower_left_corner = origin - horizontal / 2 - vertical / 2 + focal_distance * forward;

            this->lens_radius = aperture / 2;

            this->time0 = time0;
            this->time1 = time1;
        }

        ray get_ray(double u, double v) const {
            vec3d rd = lens_radius * random_in_unit_disk();
            vec3d offset = right * rd.x + up * rd.y;

            return ray(
                origin + offset, 
                lower_left_corner + u * horizontal + v * vertical - origin - offset,
                random_double(time0, time1)
            );
        }
};

#endif