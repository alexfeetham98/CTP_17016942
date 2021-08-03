#pragma once
#include "Hittable.h"
#include "Hittable_List.h"
#include <algorithm>

class BVH_Node : public Hittable
{
public:
    BVH_Node();

    BVH_Node(const Hittable_List& list, double time0, double time1)
        : BVH_Node(list.objects, 0, list.objects.size(), time0, time1) {}

    BVH_Node(const std::vector<shared_ptr<Hittable>>& src_objects,
        size_t start, size_t end, double time0, double time1);

    virtual bool Hit(
        const Ray& r, double t_min, double t_max, Hit_Record& rec) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override;

public:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB box;
};

inline bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis) {
    AABB box_a;
    AABB box_b;

    if (!a->BoundingBox(0, 0, box_a) || !b->BoundingBox(0, 0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}


bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 2);
}

BVH_Node::BVH_Node(const
    std::vector<shared_ptr<Hittable>>& src_objects,
    size_t start, size_t end, double time0, double time1)
{
    auto objects = src_objects; // Create a modifiable array of the source scene objects

    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare
        : (axis == 1) ? box_y_compare
        : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    }
    else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        }
        else {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = make_shared<BVH_Node>(objects, start, mid, time0, time1);
        right = make_shared<BVH_Node>(objects, mid, end, time0, time1);
    }

    AABB box_left, box_right;

    if (!left->BoundingBox(time0, time1, box_left)
        || !right->BoundingBox(time0, time1, box_right)
        )
        std::cerr << "No bounding box in BVH_Node constructor.\n";

    box = box.SurroundingBox(box_left, box_right);
}



bool BVH_Node::Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const {
    if (!box.Hit(r, t_min, t_max))
        return false;

    bool hit_left = left->Hit(r, t_min, t_max, rec);
    bool hit_right = right->Hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bool BVH_Node::BoundingBox(double time0, double time1, AABB& output_box) const {
    output_box = box;
    return true;
}