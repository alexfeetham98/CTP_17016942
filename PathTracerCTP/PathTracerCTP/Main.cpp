#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Float.h"
#include "Colour.h"
#include "Ray.h"
#include "Vector3.h"
#include "Camera.h"
#include "Triangle.h"
#include "RayTriangleIntercept.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Material.h"

colour ray_colour(const Ray& r, const Hittable& world, int depth/*, const Triangle& tri*/)
{
#pragma region Spheres

	/*auto l = hit_sphere(point3(0, 0, -1), 0.5, r);
	if (l > 0.0)
	{
		Vector3 N = unit_vector(r.P(l) - Vector3(0, 0, -1));
		return 0.5 * colour(N.x() + 1, N.y() + 1, N.z() + 1);
	}*/

	/*if (hit_sphere(point3(0, 0, -1), 0.1, r) > 0.0)
	{
		return colour(1, 0, 0);
	}

	if (hit_sphere(point3(1, 1, -1), 0.1, r) > 0.0)
	{
		return colour(0, 1, 0);
	}

	if (hit_sphere(point3(1, 0, -1), 0.1, r) > 0.0)
	{
		return colour(0, 0, 1);
	}*/

#pragma endregion
#pragma region RTI
	/*if (tri.RTI(r))
	{
		return colour(tri.col);
	}
	if (RayTriangleIntersect(r, tri.v0, tri.v1, tri.v2))
	{
		return colour(tri.col);
	}
	else
	{
		return colour(0, 0, 0);
	}*/
#pragma endregion

	if (depth <= 0)
	{
		return colour(0, 0, 0);
	}

	Hit_Record rec;
	if (world.Hit(r, 0.001, infinity, rec))
	{
		Ray scattered;
		colour attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * ray_colour(scattered, world, depth - 1);
		}
			
		return colour(0, 0, 0);
	}



	else
	{
		Vector3 unit_direction = unit_vector(r.direction());
		auto t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * colour(1.0, 1.0, 1.0) + t * colour(0.5, 0.7, 1.0);
	}
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
	const int sceneDepth = 50;

	image.create(nx, ny);

	//Camera
	point3 lookfrom(0, 0, -5);
	point3 lookat(0, 0, 0);
	float dist_to_focus = 10;
	float aperture = 0.0;
	Camera cam(lookfrom, lookat, Vector3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	//World
	Hittable_List world;
	auto material_ground = make_shared<Lambertian>(colour(0.8, 0.8, 0.0));
	auto material_center = make_shared<Lambertian>(colour(0.1, 0.2, 0.5));
	auto material_left = make_shared<Dielectric>(1.5);
	auto material_right = make_shared<Metal>(colour(0.8, 0.6, 0.2), 1.0);

	world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.49, material_left));
	world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
	

#pragma region Triangles
	//Front
	Triangle t1(point3(-10, -10, 10), point3(10, 10, 10), point3(-10, 10, 10), colour(1, 1, 1));
	Triangle t2(point3(-10, -10, 10), point3(10, -10, 10), point3(10, 10, 10), colour(1, 1, 1));
	//Back
	Triangle t3(point3(-10, -10, 30), point3(10, 10, 50), point3(-10, 10, 30), colour(1, 1, 1));
	Triangle t4(point3(-10, -10, 30), point3(10, -10, 30), point3(10, 10, 30), colour(1, 1, 1));
	//Right
	Triangle t5(point3(10, -10, 10), point3(10, 10, 30), point3(10, 10, 10), colour(1, 0, 0));
	Triangle t6(point3(10, -10, 10), point3(10, -10, 30), point3(10, 10, 30), colour(1, 0, 0));

	Triangle t13(point3(10, -10, 10), point3(5, 10, 30), point3(10, 10, 10), colour(1, 0, 0));
	Triangle t14(point3(-10, -10, 30), point3(-5, 10, 30), point3(-10, 10, 10), colour(0, 1, 0));

	//Left
	Triangle t7(point3(-10, -10, 30), point3(-10, 10, 30), point3(-10, 10, 10), colour(0, 1, 0));
	Triangle t8(point3(-10, -10, 30), point3(-10, 10, 10), point3(-10, -10, 10), colour(0, 1, 0));
	//Bottom
	Triangle t9(point3(-10, -10, 10), point3(10, -10, 30), point3(-10, -10, 30), colour(1, 1, 1));
	Triangle t10(point3(-10, -10, 10), point3(10, -10, 10), point3(10, -10, 30), colour(1, 1, 1));
	//Top
	Triangle t11(point3(-10, 10, 10), point3(10, 10, 30), point3(-10, 10, 30), colour(1, 1, 1));
	Triangle t12(point3(-10, 10, 10), point3(10, 10, 10), point3(10, 10, 30), colour(1, 1, 1));

	//Pyramid
	Triangle t21(point3(0, 0, 10), point3(0, 10, 20), point3(-10, 0, 20), colour(1, 1, 1));
	Triangle t22(point3(0, 0, 10), point3(10, 0, 20), point3(0, 10, 20), colour(1, 0, 0));

	point3 v0 = point3(-10, -10, 5);
	point3 v1 = point3(10, -10, 5);
	point3 v2 = point3(0, 10, 5);
	Triangle tri(v0, v1, v2, colour(1, 1, 1));

	Triangle tris[] = { t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12 };
	int o = sizeof(tris) / sizeof(tris[0]);
#pragma endregion

	//Render
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			colour col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i / float(nx));
				float v = float(j / float(ny));
				Ray r = cam.get_ray(u, v);
				col += ray_colour(r, world, sceneDepth);

				/*for (int a = 0; a < o; a++)
				{
					col += ray_colour(r, tris[a]);
				}*/

			}
			col /= float(ns);
			col = Vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			write_color(std::cout, col, ns);
			int ir = static_cast<int>(255.99 * col[0]);
			int ig = static_cast<int>(255.99 * col[1]);
			int ib = static_cast<int>(255.99 * col[2]);

			//std::cout << ir << " " << ig << " " << ib << "\n";

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