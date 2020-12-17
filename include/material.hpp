#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.hpp"
#include "hittable.hpp"

class material {
    public:
        virtual ~material() {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, 
                color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
    public:
        color albedo;

        lambertian(const color& a) : albedo { a } {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, 
            color& attenuation, ray& scattered
        ) const override {
            // true lambertian diffuse
            vec3d scatter_direction = rec.normal + random_unit_vector();
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

// class metal : public material {
//     public:
//         color albedo;

//         metal(const color& a) : albedo { a } {}


// };

#endif