#pragma once
#include <iostream>
#include <cmath>
#include <math.h>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>
#include <functional>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

#define M_PI 3.14159265358979323846264338327950288
#define kEPSILON 1e-8