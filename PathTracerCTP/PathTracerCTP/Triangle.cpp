#include "Triangle.h"

bool Triangle::RTI(const Ray& r) const
{
	//compute plane's normal
	Vector3 e0 = v1 - v0;
	Vector3 e1 = v2 - v0;

	Vector3 N = cross(e0, e1);
	float area2 = N.length();

	//Step 1: Finding P

	//check if ray and plane are parallel?
	float NdotRayDir = dot(N, r.direction());
	if (fabs(NdotRayDir) < kEPSILON) //almost 0
	{
		return false; //they are parallel so they don't intersect
	}

	//compute d parameter
	float d = dot(N, v0);

	//compute t
	double t = (dot(N, r.origin()) + d) / NdotRayDir;
	//check if the triangle is in behind the ray
	if (t < 0)
	{
		return false; // the triangle is behind
	}

	//compute the intersection point
	//Vector3 P = r.orig + t * r.dir;

	//inside-outside test
	Vector3 C; //vector perpendicular to triangle's plane

	//edge0
	Vector3 edge0 = v1 - v0;
	Vector3 vp0 = r.P(t) - v0;
	C = cross(edge0, vp0);
	if (dot(N, C) < 0)
	{
		return false; //P is on the right side
	}

	//edge 1
	Vector3 edge1 = v2 - v1;
	Vector3 vp1 = r.P(t) - v1;
	C = cross(edge1, vp1);
	if (dot(N, C) < 0)
	{
		return false; //P is on the right side
	}

	//edge 2
	Vector3 edge2 = v0 - v2;
	Vector3 vp2 = r.P(t) - v2;
	C = cross(edge2, vp2);
	if (dot(N, C) < 0)
	{
		return false; //P is on the right side
	}
	return true;
}