#pragma once
#include "Vector3.h"

class Ray
{
public:
	Ray() = default;
	Ray(const Vector3& origin, const Vector3& direction) : orig(origin), dir(direction) {}

	Vector3 origin() const { return orig; }
	Vector3 direction() const { return dir; }

	point3 point_at_param(double t) const { return orig + t * dir; }

public:
	Vector3 orig;
	Vector3 dir;
};