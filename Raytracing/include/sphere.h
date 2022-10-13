#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

class sphere : public hittable
{
public:
	sphere() {}
	// center point, radius
	sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

	virtual bool hit(
		const ray& r, double min_t, double max_t, hit_record& rec) const override;
public:
	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double min_t, double max_t, hit_record& rec) const {
	// A-C,origin - center
	vec3 oc = r.origin() - center;

	// vector dotted with itself is equal to the squared length of that vector.
	// t^2*b dot b, => direction dot direction
	auto a = r.direction().length_squared();

	// 2*t*b dot (A - C), => 2* dot((A-C), b)
	auto half_b = dot(oc, r.direction());

	// (A-C) dot (A-C) - r^2
	auto c = oc.length_squared() - radius * radius;

	auto discirminant = half_b * half_b - a * c;
	if (discirminant < 0) return false;

	auto sqrtd = sqrt(discirminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < min_t || max_t < root)
	{
		root = (-half_b + sqrtd) / a;
		if (root < min_t || max_t < root)
			return false;
	}
	rec.t = root;
	rec.p = r.at(rec.t);
	//if outward_normal is negative, the front_face is true
	//if outward_normal is position, the front_face is false
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;
	return true;
}


#endif // !SPHERE_H
