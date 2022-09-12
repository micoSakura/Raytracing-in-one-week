// Raytracing in one week.cpp: 定义应用程序的入口点。
//

#include <iostream>
#include "hittable_list.h"
#include "color.h"
#include "sphere.h"
#include "rtweekend.h"

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
color ray_color(const ray& r, const hittable& world) {
	/*auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
	if (t > 0.0)
	{
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		// map x/y/z to r/g/b
		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
	}*/

	hit_record rec;

	if (world.hit(r, 0, infinity, rec))
	{
		return 0.5 * (rec.normal + color(1, 1, 1));
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



int main()
{
	// Image
	const auto aspect_radio = 16.0 / 9.0;
	const int img_width = 400;
	const int img_height = static_cast<int>(img_width / aspect_radio);

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	// Camare
	auto viewport_height = 2.0;
	auto viewport_width = aspect_radio * viewport_height;
	auto focal_lenth = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_lenth);

	std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

	for (int j = img_height - 1; j >= 0; --j)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < img_width; ++i)
		{
			auto u = double(i) / (img_width - 1);
			auto v = double(j) / (img_height - 1);
			// called the class ray
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			color pixel_color = ray_color(r, world);
			write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";
}