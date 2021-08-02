#include "Sphere.h"

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
    rec.point = r.P(rec.t);
    Vector3 outward_normal = (rec.point - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

bool Sphere::BoundingBox(double time0, double time1, AABB& output_box) const
{
    output_box = AABB(
        center - Vector3(radius, radius, radius),
        center + Vector3(radius, radius, radius));
    return true;
}