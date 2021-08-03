#pragma once
#include "Hittable.h"

class xy_rect : public Hittable
{
public:
    xy_rect() {}

    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k,
        shared_ptr<Material> mat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

    virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z
        // dimension a small amount.
        output_box = AABB(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
        return true;
    }

public:
    shared_ptr<Material> mp;
    double x0, x1, y0, y1, k;
};

class xz_rect : public Hittable
{
public:
    xz_rect() {}

    xz_rect(double _x0, double _x1, double _z0, double _z1, double _k,
        shared_ptr<Material> mat)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

    virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Y
        // dimension a small amount.
        output_box = AABB(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
        return true;
    }

public:
    shared_ptr<Material> mp;
    double x0, x1, z0, z1, k;
};

class yz_rect : public Hittable
{
public:
    yz_rect() {}

    yz_rect(double _y0, double _y1, double _z0, double _z1, double _k,
        shared_ptr<Material> mat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

    virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const override;

    virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the X
        // dimension a small amount.
        output_box = AABB(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
        return true;
    }

public:
    shared_ptr<Material> mp;
    double y0, y1, z0, z1, k;
};