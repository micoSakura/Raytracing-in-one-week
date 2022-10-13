#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	ray() {}
	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }

	// calculation the position in a 3D line
	point3 at(double t) const {
		// ray function
		return orig + t * dir;
	}

public:
	point3 orig;
	vec3 dir;
};



#endif // !RAY_H
