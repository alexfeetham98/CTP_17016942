#pragma once
#include "Vector3.h"

class Triangle
{
public:
	Triangle() = default;
	Triangle(Vector3 cen, Vector3 verts[3]) : center(cen), verticies(verts[3]) {}
	
	

	Vector3 center;
	Vector3 verticies;
};