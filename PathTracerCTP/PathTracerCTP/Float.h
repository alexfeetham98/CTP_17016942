#pragma once
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <random>
#include <functional>

inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}

#define M_PI 3.14159265358979323846264338327950288
#define kEPSILON 1e-8