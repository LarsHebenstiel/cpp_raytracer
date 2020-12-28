#ifndef CAMERA_H
#define CAMERA_H

#include "utility.hpp"

class camera {
    public:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 forward, right, up;
        Float lens_radius;
        Float time0, time1; //open close times for the shutter

        camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            Float vert_fov,
            Float aspect_ratio,
            Float aperture,
            Float focal_distance,
            Float time0,
            Float time1
        ) {
            Float theta = degrees_to_radians(vert_fov);
            Float h = tan(theta / 2.0);
            Float view_height = 2.0 * h;
            Float view_width = aspect_ratio * view_height;
            
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

        ray get_ray(Float u, Float v) const {
            // circular aperture
            //vec3 rd = lens_radius * random_in_unit_disk();

            //triangular aperture
            point3 p0(1,0.5,0);
            point3 p1(-1,0.5,0);
            point3 p2(0,-1,0);

            p0 *= lens_radius;
            p1 *= lens_radius;
            p2 *= lens_radius;

            vec3 rd = random_in_triangle(p0, p1, p2);

            //half of the time, flip the y axis to build the star of david
            if (random_Float() > 0.5) {
                rd.y = -rd.y;
            }

            vec3 offset = right * rd.x + up * rd.y;

            return ray(
                origin + offset, 
                lower_left_corner + u * horizontal + v * vertical - origin - offset,
                random_Float(time0, time1)
            );
        }
};

#endif //CAMERA_H