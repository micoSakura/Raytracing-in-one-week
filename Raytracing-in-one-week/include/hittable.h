#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record
{
	double t;
	point3 p;
	vec3 normal;
	bool front_face;

	inline void	set_face_normal(const ray& r, const vec3& outward_normal) {
		//if dot(r.direction, outward_normal) < 0, front_face = true.
		front_face = dot(r.direction(), outward_normal) < 0;
		//if front_face is true, the ray is outside, ray and normal in the oppsite direction
		//if front_face is false, the ray is inside, ray and normal in the same direction
		normal = front_face ? outward_normal : -outward_normal;
	}
};

// abstraction
class hittable
{
public:
	//use virtual
	virtual bool hit(const ray& r, double min_t, double max_t, hit_record& rec) const = 0;
};

#endif // !HITTABLE_H