#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Float.h"
#include "Colour.h"
#include "Ray.h"
#include "Vector3.h"
#include "Camera.h"
#include "Triangle.h"
#include "Hittable_List.h"
#include "Sphere.h"




//double hit_sphere(const point3& center, double radius, const Ray& r)
//{
//	Vector3 oc = r.origin() - center;
//	auto a = r.direction().length_squared();
//	auto half_b = dot(oc, r.direction());
//	auto c = oc.length_squared() - radius * radius;
//	auto discriminant = half_b * half_b - a * c;
//	if (discriminant < 0)
//	{
//		return -1;
//	}
//	else
//	{
//		return (-half_b - sqrt(discriminant)) / a;
//	}
//}


colour ray_colour(const Ray& r, const Triangle& tri)
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

	//if (RayTriangleIntersect(r, /*v0, v1, v2,*/ triangle))
	//{
	//	return colour(1, 0, 0);
	//}
	/*Hit_Record rec;
	if (world.Hit(r, 0, INFINITY, rec))
	{
		return 0.5 * (rec.normal + colour(1, 1, 1));
	}*/

	if (tri.RTI(r))
	{
		return colour(tri.col);
	}
	else
	{
		return colour(0, 0, 0);
	}


	/*else
	{
		Vector3 unit_direction = unit_vector(r.direction());
		auto t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * colour(1.0, 1.0, 1.0) + t * colour(0.5, 0.7, 1.0);
	}*/
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
	const int ns = 1;	//number of samples per pixel (anti-aliasing)

	image.create(nx, ny);

	//Camera
	point3 lookfrom(0, 0, -50);
	point3 lookat(0, 0, 0);
	float dist_to_focus = 100;
	float aperture = 0.0;
	Camera cam(lookfrom, lookat, Vector3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	//World
	Hittable_List world;
	world.add(make_shared<Sphere>(point3(0, 0, 10), 0.5));
	//world.add(make_shared<Triangle>(point3(0, 0, 5), point3(0.5, 0.5, 5), point3(1, 0, 5)));


	//Front
	Triangle t1(point3(-10, -10, 10), point3(10, 10, 10), point3(-10, 10, 10), colour(1,1,1));
	Triangle t2(point3(-10, -10, 10), point3(10, -10, 10), point3(10, 10, 10), colour(1,1,1));
	//Back
	Triangle t3(point3(-10, -10, 30), point3(10, 10, 50), point3(-10, 10, 30), colour(1,1,1));
	Triangle t4(point3(-10, -10, 30), point3(10, -10, 30), point3(10, 10, 30), colour(1,1,1));
	//Right
	Triangle t5(point3(10, -10, 10), point3(10, 10, 30), point3(10, 10, 10), colour(1,0,0));
	Triangle t6(point3(10, -10, 10), point3(10, -10, 30), point3(10, 10, 30), colour(1,0,0));
	//Left
	Triangle t7(point3(-10, -10, 30), point3(-10, 10, 30), point3(-10, 10, 10), colour(0,1,0));
	Triangle t8(point3(-10, -10, 30), point3(-10, 10, 10), point3(-10, -10, 10), colour(0,1,0));
	//Bottom
	Triangle t9(point3(-10, -10, 10), point3(10, -10, 30), point3(-10, -10, 30), colour(1,1,1));
	Triangle t10(point3(-10, -10, 10), point3(10, -10, 10), point3(10, -10, 30), colour(1,1,1));
	//Top
	Triangle t11(point3(-10, 10, 10), point3(10, 10, 30), point3(-10, 10, 30), colour(1,1,1));
	Triangle t12(point3(-10, 10, 10), point3(10, 10, 10), point3(10, 10, 30), colour(1,1,1));
	

	point3 v0 = point3(0, 0, 5); //right
	point3 v1 = point3(0.5, 0.5, 5); //top
	point3 v2 = point3(1, 0, 5); //left
	Triangle tri(v0, v1, v2, colour(1,0,0));

	Triangle tris[] = {t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};
	int o = sizeof(tris)/sizeof(tris[0]);

	//Render
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			colour col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float t;
				float u = float(i  / float(nx));
				float v = float(j  / float(ny));
				Ray r = cam.get_ray(u, v);
				//Vector3 p = r.P(10.0);
				for (int a = 0; a < o; a++)
				{
					col += ray_colour(r, tris[a]);
				}
				
			}
			//col /= float(ns);
			//col = Vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
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