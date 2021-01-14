#ifndef SCENES_H
#define SCENES_H

#include "utility.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "moving_sphere.hpp"
#include "triangle.hpp"
#include "parse_tri_mesh.hpp"

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-500,0), 500, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_Float();
            point3 center(a + 0.9*random_Float(), 0.2, b + 0.9*random_Float());

            if ((center - point3(4, 0.2, 0)).norm() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec() * random_vec();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vec(0.5, 1);
                    auto fuzz = random_Float(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list random_moving_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-500,0), 500, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_Float();
            point3 center(a + 0.9*random_Float(), 0.2, b + 0.9*random_Float());

            if ((center - point3(4, 0.2, 0)).norm() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec() * random_vec();
                    sphere_material = make_shared<lambertian>(albedo);
                    vec3 vel(0, random_Float(0.0,0.5),0);
                    world.add(make_shared<moving_sphere>(center, vel, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vec(0.5, 1);
                    auto fuzz = random_Float(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list moving_sphere_demo() {
    hittable_list world;

    auto ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-500,0), 500, ground));

    auto mat = make_shared<lambertian>(color(0.8,0.1,0.3));
    world.add(make_shared<moving_sphere>(point3(0.0,1.0,0.0),vec3(0.0,0.5,0.0), 1,mat));

    return world;
}

hittable_list caustic_demo() {
    hittable_list world;
    
    auto ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-500,0), 500, ground));

    auto glass = make_shared<dielectric>(color(1.0,1.0,1.0),1.5);
    world.add(make_shared<sphere>(point3(0,1,0),0.5,glass));

    auto light = make_shared<diffuse_light>(color(8.0));
    world.add(make_shared<sphere>(point3(0,5,0),1,light));

    return world;
}

hittable_list emissive_lambertian_demo() {
    hittable_list world;
    
    auto ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-500,0), 500, ground));

    auto light1 = make_shared<emissive_lambertian>(color(0.2,0.1,0.8),color(0.8,0.1,0.2));
    world.add(make_shared<sphere>(point3(1,1,-1), 1, light1));

    auto light2 = make_shared<emissive_lambertian>(color(0.8,0.1,0.2),color(0.1,0.05,0.4));
    world.add(make_shared<sphere>(point3(-1,1,1), 1, light2));

    auto light3 = make_shared<emissive_lambertian>(color(1.0),color(0.2,2.2,0.6));
    world.add(make_shared<sphere>(point3(-1.5,2,-1.5), 1, light3));

    auto glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0.5,0.3,0.5), 0.3, glass));

    return world;
}

hittable_list lights_scene() {
    hittable_list world;

    auto ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-500,0), 500, ground));

    for (int i = 0; i < 30; i++) {
        auto light = make_shared<diffuse_light>(color(80.0,70.0,60.0) + 10 * random_unit_vector());
        Float x = random_Float(-12,5);
        Float z = random_Float(-12,5);
        Float y = 2.75 + random_Float(0,1) + x / 15 + z / 15;
        Float radius = random_Float(0.1,0.4);
        radius *= radius; //make more smaller radius lights
        world.add(make_shared<sphere>(point3(x,y,z), radius, light));
    }

    for (int i = 0; i < 5; i++) {
        auto light = make_shared<diffuse_light>(1.75 * color(80.0,70.0,60.0) + 10 * random_unit_vector());
        world.add(make_shared<sphere>(point3(5 - i, 2.75, i), 0.05,light));
    }

    auto red = make_shared<lambertian>(color(0.8,0.15,0.1));
    world.add(make_shared<sphere>(point3(0,0.65,0),1,red));

    auto white = make_shared<lambertian>(color(1.0));
    world.add(make_shared<sphere>(point3(0.7,1.1,0.5),0.4,white));
    world.add(make_shared<sphere>(point3(0.7,1.1,-0.5),0.4,white));

    auto black = make_shared<lambertian>(color(0.0));
    world.add(make_shared<sphere>(point3(0.81,1.1,0.5),0.3,black));
    world.add(make_shared<sphere>(point3(0.81,1.1,-0.5),0.3,black));

    auto green = make_shared<lambertian>(color(0.1,0.8,0.2));
    world.add(make_shared<sphere>(point3(1.2,0.45,0.1), 0.45, green));

    auto blue = make_shared<lambertian>(color(0.05,0.1,0.8));
    for (int i = 0; i < 3; i++) {
        world.add(make_shared<sphere>(point3(0.35, 0.6, 0.9 * i + 1.5), 0.35, blue));
    }
    vec3 vel(-0.4, 0 , -0.9);
    vel *= 1.2;
    for (int i = 0; i < 8; i++) {
        world.add(make_shared<sphere>(point3(1.35, 0.6, -1.5) + i * vel, 0.35, blue));
    }

    return world;
}

hittable_list debug_bvh_sphere() {
    hittable_list world;

    auto mat = make_shared<lambertian>(color(0.8,0.05,0.1));
    world.add(make_shared<sphere>(point3(0.0),1.0,mat));
    world.add(make_shared<sphere>(point3(0.0, 3.0, 0.0),1.0,mat));
    world.add(make_shared<sphere>(point3(-1.0, 1.5, 1.0),1.0,mat));
    world.add(make_shared<sphere>(point3(1.0, 1.5, -1.0),1.0,mat));

    return world;
}

std::vector<shared_ptr<triangle>> test_tri_mesh(shared_ptr<material> mat_ptr) {
    std::vector<shared_ptr<triangle>> vec;

    const int nTriangles = 1;
    const int nVertices = 3;

    int * vertex_indicies = new int[3] {0,2,1};
    point3 * p = new point3[3] {point3(0.0), point3(1.0,0.0,0.0), point3(0.0,0.0,1.0)};
    vec3 * n = new vec3[3] {unit_vector(vec3(0.0, 1.0, 0.0)), unit_vector(vec3(0.0, 1.0, 0.0)), unit_vector(vec3(0.0, 1.0, 0.0))};

    auto mesh = make_shared<TriangleMesh>(nTriangles, vertex_indicies, nVertices, p, n, mat_ptr);
    
    vec.push_back(make_shared<triangle>(mesh, 0));

    delete[] vertex_indicies;
    delete[] p;
    delete[] n;

    return vec;
}

hittable_list test_obj_file(const std::string& filename, std::ostream& log) {
    hittable_list world;

    auto ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-501,0), 500, ground));

    auto rose_mirror = make_shared<metal>(color(1.0,0.8,0.9));
    auto red = make_shared<lambertian>(color(0.8,0.05,0.1));
    std::vector<shared_ptr<triangle>> v = build_mesh(filename, red, log);

    for (const auto& h : v) world.add(h);

    return world;
}

#endif // SCENES_H