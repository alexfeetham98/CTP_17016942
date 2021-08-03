#pragma once
#include "Vector3.h"
#include "Ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB
{
public:
    AABB() {}
    AABB(const point3& a, const point3& b) { minimum = a; maximum = b; }

    point3 min() const { return minimum; }
    point3 max() const { return maximum; }

    bool Hit(const Ray& r, double t_min, double t_max) const
    {
        for (int a = 0; a < 3; a++) {
            auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
                            (maximum[a] - r.origin()[a]) / r.direction()[a]);
            auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
                            (maximum[a] - r.origin()[a]) / r.direction()[a]);
            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    point3 minimum;
    point3 maximum;
    AABB SurroundingBox(AABB box0, AABB box1)
    {
        point3 small(ffmin(box0.min().x(), box1.min().x()),
            ffmin(box0.min().y(), box1.min().y()),
            fmin(box0.min().z(), box1.min().z()));

        point3 big(fmax(box0.max().x(), box1.max().x()),
            ffmax(box0.max().y(), box1.max().y()),
            ffmax(box0.max().z(), box1.max().z()));

        return AABB(small, big);
    }
};

//AABB AABB::SurroundingBox(AABB box0, AABB box1)
//{
//    point3 small(ffmin(box0.min().x(), box1.min().x()),
//        ffmin(box0.min().y(), box1.min().y()),
//        fmin(box0.min().z(), box1.min().z()));
//
//    point3 big(fmax(box0.max().x(), box1.max().x()),
//        ffmax(box0.max().y(), box1.max().y()),
//        ffmax(box0.max().z(), box1.max().z()));
//
//    return AABB(small, big);
//}