#pragma once
#include "Vector3.h"
#include "Ray.h"
#include "Float.h"

class Triangle
{
public:
	Triangle() = default;
	Triangle(point3 V0, point3 V1, point3 V2, colour Colour) : v0(V0), v1(V1), v2(V2), col(Colour) {};

	bool RTI(const Ray& r) const;
public:
	Vector3 center;
	point3 v0;
	point3 v1;
	point3 v2;
	colour col;
};