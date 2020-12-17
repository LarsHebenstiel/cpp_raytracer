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

            // catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal : public material {
    public:
        color albedo;
        double fuzz;

        metal(const color& a) : albedo { a }, fuzz { 0 } {}
        metal(const color& a, double f) : albedo { a }, fuzz { f < 1.0? f : 1.0 } {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, 
            color& attenuation, ray& scattered
        ) const override {
            vec3d reflected = reflect(unit_vector(r_in.dir), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};

#endif