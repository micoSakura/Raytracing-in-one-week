// Raytracing in one week.cpp: 定义应用程序的入口点。
//

#include <iostream>
#include "hittable_list.h"
#include "color.h"
#include "sphere.h"
#include "rtweekend.h"
#include "camera.h"
#include "material.h"

// calculate the Ray-sphere intersection results
/*double hit_sphere(const point3& center, double radius, const ray& r) {
	// A-C,origin - center
	vec3 oc = r.origin() - center;

	// vector dotted with itself is equal to the squared length of that vector.
	// t^2*b dot b, => direction dot direction
	//auto a = dot(r.direction(), r.direction());
	auto a = r.direction().length_squared();

	// 2*t*b dot (A - C), => 2* dot((A-C), b)
	//auto b = 2.0 * dot(oc, r.direction());
	auto half_b = dot(oc, r.direction());

	// (A-C) dot (A-C) - r^2
	//auto c = dot(oc, oc) - radius * radius;
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	// if discriminant < 0, none root.
	if (discriminant < 0)
	{
		return -1.0;
	}
	else// map from 0 to 1
	{
		// p - c
		return (-half_b - sqrt(discriminant)) / a;
	}
}*/

// ray color
color ray_color(const ray& r, const hittable& world, int depth) {
	/*auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
	if (t > 0.0)
	{
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		// map x/y/z to r/g/b
		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
	}*/

	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
	{
		return color(0, 0, 0);
	}

	if (world.hit(r, 0.001, infinity, rec))
	{
		ray scattered;
		color attenuation;
		// point3 target = rec.p + random_in_hemisphere(rec.normal);
		// return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
		// return 0.5 * (rec.normal + color(1, 1, 1));

		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);

	}

	// linear interpolation
	vec3 unit_direction = unit_vector(r.direction());
	// -1.0 < y < 1.0 from the y axis,
	// if y = 1, the t = 1,
	// if y = -1, the t = 0.
	// normalize(t)
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


hittable_list ramdom_scene() {
	hittable_list world;
	auto ground_matelrial = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_matelrial));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9)
			{
				shared_ptr<material> sphere_material;
				if (choose_mat < 0.8)
				{
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95)
				{
					// matel
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<matel>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else
				{
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}
	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<matel>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
	
	return world;
}



int main()
{
	// Image
	const auto aspect_radio = 3.0 / 2.0;
	const int img_width = 1200;
	const int img_height = static_cast<int>(img_width / aspect_radio);
	const int smaples_per_pixel = 500;
	const int max_depth = 50;

	// World
	//hittable_list world;

	auto world = ramdom_scene();
	/*auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<matel>(color(0.8, 0.6, 0.2), 0.0);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));*/

	/*auto r = cos(pi / 4);
	auto material_left = make_shared<lambertian>(color(0, 0, 1));
	auto material_right = make_shared<lambertian>(color(1, 0, 0));
	world.add(make_shared<sphere>(point3(-r, 0, -1), r, material_left));
	world.add(make_shared<sphere>(point3(r, 0, -1), r, material_right));*/

	// Camare
	//camera cam(90.0, aspect_radio);
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;
	camera cam(lookfrom, lookat, vup, 20.0, aspect_radio, aperture, dist_to_focus);

	/*auto viewport_height = 2.0;
	auto viewport_width = aspect_radio * viewport_height;
	auto focal_lenth = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_lenth);*/

	std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

	for (int j = img_height - 1; j >= 0; --j)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < img_width; ++i)
		{
			color pixel_color(0, 0, 0);
			for (int s = 0; s < smaples_per_pixel; ++s)
			{
				auto u = (i + random_double()) / (img_width - 1);
				auto v = (j + random_double()) / (img_height - 1);
				// called the class ray
				//ray r(origin, lower_left_corner + u * horizontal + v * vertical);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color, smaples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}
