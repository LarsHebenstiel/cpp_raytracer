#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.hpp"
#include "hittable.hpp"
#include <iostream>

class material {
    public:
        virtual ~material() {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, 
                color& attenuation, ray& scattered) const = 0;
        
        virtual color emitted() const {
            return color(0.0);
        }
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

            scattered = ray(rec.p, scatter_direction, r_in.time);
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
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};

class dielectric : public material {
    public:
        color albedo;
        double ir; //index of refraction

        dielectric(const color& c) : albedo { c }, ir { 1.0 } {}
        dielectric(const color& c, double index_of_refraction) : albedo { c }, ir { index_of_refraction } {}
        dielectric(double index_of_refraction) : albedo { color(1.0) }, ir { index_of_refraction } {}

        static double schlick_reflectance(double cos, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            double res = r0 + (1 - r0) * pow((1 - cos),5);
            return res;
        }

        virtual bool scatter(const ray& r_in, const hit_record& rec, 
            color& attenuation, ray& scattered
        ) const override {
            attenuation = albedo;
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
            vec3d unit_direction = unit_vector(r_in.dir);
            
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;

            vec3d direction;

            if (cannot_refract || schlick_reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction, r_in.time);
            return true;
        }
        
};

class diffuse_light : public material {
    public:
        color emit;

        diffuse_light(color c) : emit { c } {}
        diffuse_light() : emit { color(1.0) } {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, 
            color& attenuation, ray& scattered
        ) const override {
            return false;
        }

        virtual color emitted() const override {
            return emit;
        }

};

#endif