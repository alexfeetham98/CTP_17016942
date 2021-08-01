#pragma once
#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class Hittable_List : public Hittable
{
public:
    Hittable_List() = default;
    Hittable_List(shared_ptr<Hittable> object) { add(object); }
    ~Hittable_List() = default;

    void clear() { objects.clear(); }
    void add(shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const override;
    //virtual bool RTI(const Ray& r, double t_min, double t_max, Hit_Record& rec) const override;

public:
    std::vector<shared_ptr<Hittable>> objects;
};

bool Hittable_List::Hit(const Ray& r, double t_min, double t_max, Hit_Record& rec) const
{
    Hit_Record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects)
    {
        if (object->Hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

//bool Hittable_List::RTI(const Ray& r, double t_min, double t_max, Hit_Record& rec) const
//{
//
//    Hit_Record temp_rec;
//    bool hit_anything = false;
//    auto closest_so_far = t_max;
//
//    for (const auto& object : objects)
//    {
//        if (object->RTI(r, t_min, closest_so_far, temp_rec))
//        {
//            hit_anything = true;
//            closest_so_far = temp_rec.t;
//            rec = temp_rec;
//        }
//    }
//    return hit_anything;
//}