#pragma once
#include "Utilities.h"
#include "Vector3.h"
#include "Ray.h"
#include "AARect.h"
#include "Hittable_List.h"

class Box : public Hittable
{
public:
    Box() {}
    Box(const point3& p0, const point3& p1, shared_ptr<Material> ptr);

    virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override {
        output_box = AABB(box_min, box_max);
        return true;
    }

public:
    point3 box_min;
    point3 box_max;
    Hittable_List sides;
};

Box::Box(const point3& p0, const point3& p1, shared_ptr<Material> ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool Box::Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const
{
    return sides.Hit(r, t_min, t_max, rec);
}