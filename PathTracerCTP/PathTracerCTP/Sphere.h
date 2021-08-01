#pragma once
#include "Hittable.h"
#include "Vector3.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
	Sphere(point3 cen, double r) : center(cen), radius(r) {};
    ~Sphere() = default;

	bool Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const;

public:
	point3 center;
	double radius;
};