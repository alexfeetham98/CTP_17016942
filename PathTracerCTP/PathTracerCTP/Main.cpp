#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Utilities.h"
#include "Colour.h"
#include "Ray.h"
#include "Vector3.h"
#include "Camera.h"
#include "Triangle.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Material.h"
#include "AABB.h"
#include "BVH.h"
#include "AArect.h"
#include "Box.h"

colour ray_colour(const Ray& r, const colour& background, const Hittable& world, /*const Triangle& tri,*/ int depth)
{
#pragma region RTI
	//if (tri.RTI(r))
	//{
	//	return colour(tri.col);
	//}
#pragma endregion

	if (depth <= 0)
	{
		return colour(0, 0, 0);
	}

	Hit_Record rec;
	// If the ray hits nothing, return the background color.
	if (!world.Hit(r, 0.001, infinity, rec))
	{
		return background;
	}	

	Ray scattered;
	colour attenuation;
	colour emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.point);

	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
	{
		return emitted;
	}

	return emitted + attenuation * ray_colour(scattered, background, world, depth - 1);
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

Hittable_List scene()
{
	//World
	Hittable_List world;
	auto material_ground = make_shared<Lambertian>(colour(.73, .73, .73));
	auto material_center = make_shared<Lambertian>(colour(0.1, 0.2, 0.5));
	auto material_left = make_shared<Dielectric>(1.5);
	auto material_right = make_shared<Metal>(colour(0.8, 0.6, 0.2), 0.0);

	auto checker = make_shared<checker_texture>(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9));
	world.add(make_shared<Sphere>(point3(0, -100.5, 0), 100, material_ground));

	world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.49, material_left));
	world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	return world;
}

Hittable_List two_spheres()
{
	Hittable_List objects;

	auto checker = make_shared<checker_texture>(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9));

	objects.add(make_shared<Sphere>(point3(0, -10, 0), 10, make_shared<Lambertian>(checker)));
	objects.add(make_shared<Sphere>(point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));

	return objects;
}

Hittable_List two_perlin_spheres()
{
	Hittable_List objects;

	auto pertext = make_shared<Noise_Texture>(4);
	objects.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
	objects.add(make_shared<Sphere>(point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

	return objects;
}

Hittable_List simple_light()
{
	Hittable_List objects;

	auto pertext = make_shared<Noise_Texture>(4);
	objects.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
	objects.add(make_shared<Sphere>(point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

	auto difflight = make_shared<Diffuse_Light>(colour(4, 4, 4));
	objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

	return objects;
}

Hittable_List cornell_box()
{
	Hittable_List objects;

	auto red = make_shared<Lambertian>(colour(.65, .05, .05));
	auto white = make_shared<Lambertian>(colour(.73, .73, .73));
	auto green = make_shared<Lambertian>(colour(.12, .45, .15));
	auto light = make_shared<Diffuse_Light>(colour(15, 15, 15));
	auto metal = make_shared<Metal>(colour(1, 1, 1), 0.0);

	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

	objects.add(make_shared<Box>(point3(130, 0, 65), point3(295, 165, 230), white));
	objects.add(make_shared<Box>(point3(265, 0, 295), point3(430, 330, 460), white));

	return objects;
}

int main()
{
	Timer timer;
	timer.Start();
	sf::RenderWindow window(sf::VideoMode(800, 600), "Path Tracer");

	sf::Event event;
	sf::Image image;

	const int nx = 800;		//image width
	const int ny = 600;		//image height
	const int ns = 10000;	//number of samples per pixel (anti-aliasing)
	const int sceneDepth = 50;

	image.create(nx, ny);

	Hittable_List world = cornell_box();
	colour background(0, 0, 0);

	//Camera
	point3 lookfrom(280, 280, -800);
	point3 lookat(280, 280, 0);
	float dist_to_focus = 10;
	float aperture = 0.0;
	Camera cam(lookfrom, lookat, Vector3(0, 1, 0), 40, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

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
	//Left
	Triangle t7(point3(-10, -10, 30), point3(-10, 10, 30), point3(-10, 10, 10), colour(0, 1, 0));
	Triangle t8(point3(-10, -10, 30), point3(-10, 10, 10), point3(-10, -10, 10), colour(0, 1, 0));
	//Bottom
	Triangle t9(point3(-10, -10, 10), point3(10, -10, 30), point3(-10, -10, 30), colour(1, 1, 1));
	Triangle t10(point3(-10, -10, 10), point3(10, -10, 10), point3(10, -10, 30), colour(1, 1, 1));
	//Top
	Triangle t11(point3(-10, 10, 10), point3(10, 10, 30), point3(-10, 10, 30), colour(1, 1, 1));
	Triangle t12(point3(-10, 10, 10), point3(10, 10, 10), point3(10, 10, 30), colour(1, 1, 1));

	Triangle tris[] = { t3, t4, t5, t6, t7, t8, t9, t10, t11, t12 };

	int o = sizeof(tris) / sizeof(tris[0]);
#pragma endregion

	std::cerr << "\Number of Samples: " << ns << "\n";

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
				col += ray_colour(r, background, world, sceneDepth);
			}
			col /= float(ns);
			col = Vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			write_color(std::cout, col, ns, timer);
			int ir = static_cast<int>(255.99 * col[0]);
			int ig = static_cast<int>(255.99 * col[1]);
			int ib = static_cast<int>(255.99 * col[2]);

			//std::cout << ir << " " << ig << " " << ib << "\n";

			image.setPixel(i, j, sf::Color(ir, ig, ib));
		}
	}

	std::cerr << "\Done.\n";
	std::cerr << "\Time Elapsed: " << timer.GetDuration() << "\n";
	std::cerr << "\Number of Samples: " << ns << "\n";

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