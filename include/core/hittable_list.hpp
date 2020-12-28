#ifndef HITTABLE_LIST
#define HITTABLE_LIST

#include <cmath>
#include <memory>
#include <vector>

#include "utility.hpp"
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
            const ray& r, Float t_min, Float t_max, hit_record& rec) const override;

        virtual bool bounding_box(Float time0, Float time1, aabb& output_box) const override;
};

bool hittable_list::hit(const ray& r, Float t_min, Float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    Float closest_hit = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_hit, temp_rec)) {
            hit_anything = true;
            closest_hit= temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(Float time0, Float time1, aabb& output_box) const  {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first ? temp_box : surrounding_box(output_box, temp_box);
        first = false;
    }

    return true;
}

#endif //HITTABLE_LIST