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
            vec3 scatter_direction = rec.normal + random_unit_vector();

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
        Float fuzz;

        metal(const color& a) : albedo { a }, fuzz { 0 } {}
        metal(const color& a, Float f) : albedo { a }, fuzz { f < 1? f : 1 } {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, 
            color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.dir), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};

class dielectric : public material {
    public:
        color albedo;
        Float ir; //index of refraction

        dielectric(const color& c) : albedo { c }, ir { 1.0 } {}
        dielectric(const color& c, Float index_of_refraction) : albedo { c }, ir { index_of_refraction } {}
        dielectric(Float index_of_refraction) : albedo { color(1.0) }, ir { index_of_refraction } {}

        static Float schlick_reflectance(Float cos, Float ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            Float res = r0 + (1 - r0) * pow((1 - cos),5);
            return res;
        }

        virtual bool scatter(const ray& r_in, const hit_record& rec, 
            color& attenuation, ray& scattered
        ) const override {
            attenuation = albedo;
            Float refraction_ratio = rec.front_face ? (1.0/ir) : ir;
            vec3 unit_direction = unit_vector(r_in.dir);
            
            Float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            Float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;

            vec3 direction;

            if (cannot_refract || schlick_reflectance(cos_theta, refraction_ratio) > random_Float())
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

        virtual bool scatter(const ray&, const hit_record&, color&, ray& ) const override {
            return false;
        }

        virtual color emitted() const override {
            return emit;
        }

};

class emissive_lambertian : public lambertian {
    public:
        color emit;

        emissive_lambertian(const color& a, const color& e) : lambertian(a), emit{ e } {}

        virtual color emitted() const override {
            return emit;
        }
};

class emissive_metal : public metal {
    public:
        color emit;

        emissive_metal(const color& a, const color&e) : metal(a), emit{ e } {}
        emissive_metal(const color& a, const color&e, Float f) : metal(a, f), emit{ e } {}

        virtual color emitted() const override {
            return emit;
        }
};

#endif //MATERIAL_H