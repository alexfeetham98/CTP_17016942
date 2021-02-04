#pragma once
#include "Hittable.h"
#include "Vector3.h"

class Sphere : public Hittable
{
public:
	Sphere() {}
	Sphere(point3 cen, double r) : center(cen), radius(r) {};

	virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const override;
public:
	point3 center;
	double radius;
};

bool Sphere::Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const
{
    Vector3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.P(rec.t);
    Vector3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}