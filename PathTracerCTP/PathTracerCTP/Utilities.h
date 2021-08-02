#pragma once
#include <iostream>
#include <cmath>
#include <math.h>
#include <limits>
#include <memory>
#include <cstdlib>
#include <functional>
#include <random>
#include <chrono>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

inline int random_int(int min, int max)
{
	return static_cast<int>(random_double(min, max + 1));
}

inline double random_double()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

#define M_PI 3.14159265358979323846264338327950288
#define kEPSILON 1e-8

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> m_StartTime;

public:
	void Start(){ m_StartTime = std::chrono::high_resolution_clock::now(); }
	float GetDuration()
	{
		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_StartTime;
		return (duration.count() / 60);
	}
};