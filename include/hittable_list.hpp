#ifndef HITTABLE_LIST
#define HITTABLE LIST

#include <cmath>
#include <memory>
#include <vector>

#include "vec3d.hpp"
#include "ray.hpp"
#include "hittable.hpp"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_hit = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_hit, temp_rec)) {
            hit_anything = true;
            closest_hit= temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif