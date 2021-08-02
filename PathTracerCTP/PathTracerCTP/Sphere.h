#pragma once
#include "Hittable.h"
#include "Vector3.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
	Sphere(point3 cen, double r, shared_ptr<Material> m) : center(cen), radius(r), mat_ptr(m) {};
    ~Sphere() = default;

	virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const override;
	virtual bool BoundingBox(double time0, double time1, AABB& output_box) const override;

public:
	point3 center;
	double radius;
	shared_ptr<Material> mat_ptr;
};