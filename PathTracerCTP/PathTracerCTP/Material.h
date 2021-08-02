#pragma once
#include "Ray.h"
#include "Hittable.h"
#include "Float.h"

struct Hit_Record;

Vector3 reflect(const Vector3& v, const Vector3& n)
{
	return v - 2 * dot(v, n) * n;
}

Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat)
{
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const Hit_Record& rec, colour& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const colour& a) : albedo(a) {}

	virtual bool scatter(const Ray& r_in, const Hit_Record& rec, colour& attenuation, Ray& scattered) const override
	{
		/*auto scatter_direction = rec.normal + random_unit_vector();
		if (scatter_direction.near_zero())
		{
			scatter_direction = rec.normal;
		}			
		scattered = Ray(rec.point, scatter_direction);
		attenuation = albedo;
		return true;*/
		Vector3 target = rec.point + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.point, target - rec.point);
		attenuation = albedo;
		return true;
	}
	
public:
	colour albedo;
};

class Metal : public Material {
public:
	Metal(const colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(const Ray& r_in, const Hit_Record& rec, colour& attenuation, Ray& scattered) const override
	{
		Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.point, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	colour albedo;
	double fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(const Ray& r_in, const Hit_Record& rec, colour& attenuation, Ray& scattered) const override
	{
		attenuation = colour(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		Vector3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		Vector3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
		{
			direction = reflect(unit_direction, rec.normal);
		}			
		else
		{
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		scattered = Ray(rec.point, direction);
		return true;
	}

public:
	double ir; // Index of Refraction
private:
	static double reflectance(double cosine, double ref_idx)
	{
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
