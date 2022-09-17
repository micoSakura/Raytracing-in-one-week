#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

// get from the lib
using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{
public:
	hittable_list() {}
	// shared_ptr<type> point to newly-allocated object
	// make_shared<thing>(thing_constructor_params ...) allocted a new instance of type thing
	// ,using the constructor parameters.It is a shared_ptr<thing>.
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clean() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(
		const ray& r, double min_t, double max_t, hit_record& rec) const override;

public:
	std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double min_t, double max_t, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closet_so_far = max_t;
	// Traverse objects that can be hit by light
	for (const auto& object : objects)
	{
		// if hit object, hit_anything is true.
		if (object->hit(r, min_t, closet_so_far, temp_rec))
		{
			hit_anything = true;
			closet_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

#endif // !HITTABLE_LIST_H
