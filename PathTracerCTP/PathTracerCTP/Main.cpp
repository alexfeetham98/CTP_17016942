#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Float.h"
#include "Colour.h"
#include "Ray.h"
#include "Vector3.h"
#include "Camera.h"
#include "Hittable_List.h"
#include "Sphere.h"

double hit_sphere(const point3& center, double radius, const Ray& r)
{
	Vector3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
	{
		return -1;
	}
	else
	{
		return (-half_b - sqrt(discriminant)) / a;
	}
}

bool RayTriangleIntersect(const Ray& r, const point3& v0, const point3& v1, const point3& v2)
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
	Vector3 P = r.orig + t * r.dir;

	//inside-outside test
	Vector3 C; //vector perpendicular to triangle's plane

	//edge0
	Vector3 edge0 = v1 - v0;
	Vector3 vp0 = P - v0;
	C = cross(edge0, vp0);
	if (dot(N, C) < 0)
	{
		return false; //P is on the right side
	}

	//edge 1
	Vector3 edge1 = v2 - v1;
	Vector3 vp1 = P - v1;
	C = cross(edge1, vp1);
	if (dot(N, C) < 0)
	{
		return false; //P is on the right side
	}

	//edge 2
	Vector3 edge2 = v0 - v2;
	Vector3 vp2 = P - v2;
	C = cross(edge2, vp2);
	if (dot(N, C) < 0)
	{
		return false; //P is on the right side
	}

	return true;
}

colour ray_colour(const Ray& r)
{
	/*Hit_Record rec;
	if (world.Hit(r, 0, INFINITY, rec))
	{
		return 0.5 * (rec.normal + colour(1, 1, 1));
	}*/

	#pragma region Spheres

	if (hit_sphere(point3(-4, 0, -1), 0.5, r) > 0.0)
	{
		return colour(1, 0, 0);
	}

	auto l = hit_sphere(point3(0, 0, -1), 0.5, r);
	if (l > 0.0)
	{
		Vector3 N = unit_vector(r.P(l) - Vector3(0, 0, -1));
		return 0.5 * colour(N.x() + 1, N.y() + 1, N.z() + 1);
	}

	if (hit_sphere(point3(0, -100.5, -1), 100, r) > 0.0)
	{
		return colour(0.2, 0.8, 0.4);
	}

	#pragma endregion

	/*if (RayTriangleIntersect(r, point3(0, 1, -1), point3(1, 1, -1), point3(1, 2, -1)))
	{
		return colour(1, 0, 0);
	}*/

	Vector3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * colour(1.0, 1.0, 1.0) + t * colour(0.5, 0.7, 1.0);
}

Vector3 normalize(Vector3 &A, Vector3 &B, Vector3 &C)
{
	float x, y, z;
	x = A.y() * B.z() - A.z() * B.y();
	y = A.z() * B.x() - A.x() * B.z();
	z = A.x() * B.y() - A.y() * B.x();
	C = Vector3(x, y, z);
	return C;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Path Tracer");

	sf::Event event;
	sf::Image image;
	
	const int nx = 800; //image width
	const int ny = 600; //image height
	const int ns = 10;	//number of samples per pixel (anti-aliasing)

	image.create(nx, ny);

	//Camera
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	float dist_to_focus = 10;
	float aperture = 0.0;
	Camera cam(lookfrom, lookat, Vector3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	//Render
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			colour col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i  / float(nx));
				float v = float(j  / float(ny));
				Ray r = cam.get_ray(u, v);
				Vector3 p = r.P(2.0);
				col += ray_colour(r);
			}
			col /= float(ns);
			col = Vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = static_cast<int>(255.99 * col[0]);
			int ig = static_cast<int>(255.99 * col[1]);
			int ib = static_cast<int>(255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";

			image.setPixel(i, j, sf::Color(ir, ig, ib));
		}
	}

	std::cerr << "\Done.\n";

	image.flipVertically();
	while (window.isOpen())
	{
		window.clear();
		sf::Texture texture;
		texture.loadFromImage(image);

		sf::Sprite sprite;
		sprite.setTexture(texture, true);

		window.draw(sprite);

		window.display();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}
	return 0;
}