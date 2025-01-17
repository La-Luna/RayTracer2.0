#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H
#include "hitable.h"
#include "ray.h"
#include "aabb.h"
#include <vector>
#include <memory>
using std::shared_ptr;
using std::make_shared;

class hitable_list :public hitable{
public:
	hitable_list(){};
	hitable_list(shared_ptr<hitable> object){ add(object); }

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const override;
	void clear(){ objects.clear(); }
	void add(shared_ptr<hitable> object){ objects.push_back(object); }


	//hitable**list;
	std::vector<shared_ptr<hitable>> objects;
};
bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec)const{
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (const auto&object:objects){
		if (object->hit(r, t_min, closest_so_far, temp_rec)){
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}
bool hitable_list::bounding_box(double t0, double t1, aabb& out_box)const{
	if (objects.empty())return false;
	aabb temp_box;
	bool first_box = true;
	for (const auto&object : objects){
		if (!object->bounding_box(t0, t1, temp_box))return false;
		out_box = first_box ? temp_box : surrounding_box(out_box, temp_box);
		first_box = false;
	}
	return true;

}


#endif
