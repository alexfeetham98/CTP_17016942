#pragma once
#include <memory>
#include "Ray.h"

class Material;

struct Hit_Record
{
    point3 point;
    Vector3 normal;
    shared_ptr<Material>mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const Ray& r, const Vector3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : (outward_normal * -1);
    }
};

class Hittable
{
public:
    virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const = 0;
};