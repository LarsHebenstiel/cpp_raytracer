#ifndef BVH_NODE_H
#define BVH_NODE_H

#include <vector> 
#include <algorithm>
#include <iostream>

#include "utility.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

class bvh_node : public hittable {
    public:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        aabb box;

        bvh_node();
        bvh_node(const hittable_list& list, Float time0, Float time1)
        : bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}

        bvh_node(
            const std::vector<shared_ptr<hittable>>& objs,
            int start, int end, Float time0, Float time1, int axis = 0
        );

        virtual bool hit(const ray& r, Float t_min, Float t_max, hit_record& rec) const override;

        virtual bool bounding_box(Float time0, Float time1, aabb& output_box) const override;
};

bool bvh_node::bounding_box(Float, Float, aabb& output_box) const {
    output_box = box;
    return true;
}

bool bvh_node::hit(const ray& r, Float t_min, Float t_max, hit_record& rec) const {
    if(!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb ba, bb;

    if (!a->bounding_box(0,0, ba) || !b->bounding_box(0,0, bb))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    if (axis == 0) {
        return ba.min.x < bb.min.x;
    } else if (axis == 1) {
        return ba.min.y < bb.min.y;
    } else {
        return ba.min.z < bb.min.z;
    }
}

bool box_compare_x (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_compare_y (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_compare_z (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

bvh_node::bvh_node(
    const std::vector<shared_ptr<hittable>>& objs,
    int start, int end, Float time0, Float time1, int axis
) {
    auto objects = objs;

    auto comparator = axis == 0 ? box_compare_x 
                    : axis == 1 ? box_compare_y 
                                : box_compare_z;

    int span = end - start;

    if (span == 1) {
        left = right = objects[start];
    } else if (span == 2) {
        // order doesn't matter
        left = objects[start];
        right = objects[start + 1];
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        int mid = start + span / 2;
        left = make_shared<bvh_node>(objects, start, mid, time0, time1, (axis + 1) % 3);
        right = make_shared<bvh_node>(objects, mid, end, time0, time1, (axis + 1) % 3);
    }

    aabb box_l, box_r;

    if (  ! left->bounding_box(time0, time1, box_l)
       || ! right->bounding_box(time0, time1, box_r)
    )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surrounding_box(box_l, box_r);
}

#endif //BVH_NODE_H