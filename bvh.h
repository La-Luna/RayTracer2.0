#ifndef BVH_H
#define BVH_H
#include "hitable.h"
#include "aabb.h"
#include "hitable_list.h"
#include "rtweekend.h"
#include <vector>
#include <algorithm>
class bvh_node:public hitable{

public:
	bvh_node(){};
	bvh_node(const hitable_list& list, double t0, double t1):bvh_node(list.objects, 0, list.objects.size(), t0, t1)
	{};
	bvh_node(std::vector<shared_ptr<hitable>> objects, size_t start, size_t end, double t0, double t1);
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const override;

	shared_ptr<hitable>left;
	shared_ptr<hitable>right;
	



	aabb box;

};
inline bool box_compare(const shared_ptr<hitable> a, const shared_ptr<hitable> b, int axis){
	aabb box_a;
	aabb box_b;
	if (!a->bounding_box(0.0, 0.0, box_a) || !b->bounding_box(0.0, 0.0, box_b)){
		std::cerr << "No bounding box in bvh_node constructor!\n";
	}
	return box_a.min().e[axis] < box_b.min().e[axis];

}
bool box_x_compare(const shared_ptr<hitable> a, const shared_ptr<hitable> b){ return box_compare(a, b, 0); }
bool box_y_compare(const shared_ptr<hitable> a, const shared_ptr<hitable> b){ return box_compare(a, b, 1); }
bool box_z_compare(const shared_ptr<hitable> a, const shared_ptr<hitable> b){ return box_compare(a, b, 2); }
bvh_node::bvh_node(std::vector<shared_ptr<hitable>>objects,size_t start,size_t end,double time0,double time1){
	int axis = random_int(0, 2);
	auto mycomparator = (axis == 0) ? box_x_compare
		: (axis == 1) ? box_y_compare
		: box_z_compare;
	size_t object_span = end-start;
	if (object_span == 1){
		left = right = objects[start];
	}
	else if (object_span == 2){
		if (mycomparator(objects[start], objects[start + 1])){
			left = objects[start];
			right = objects[start + 1];
		}
		else{
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else{
		std::sort(objects.begin() + start, objects.begin() + end, mycomparator);
		
		auto mid = start + object_span / 2;

		left = make_shared<bvh_node>(objects, start, mid, time0, time1);
		right = make_shared<bvh_node>(objects, mid, end, time0,time1);
	}
	aabb box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)){
		std::cerr << "no " << std::endl;


	}
	box = surrounding_box(box_left, box_right);




}


bool bvh_node::hit(const ray& r, float t_min, float t_max, hit_record& rec)const{
	if (!box.hit(r, t_min, t_max))return false;
	bool lefthit = left->hit(r, t_min, t_max, rec);
	bool righthit = right->hit(r, t_min, lefthit ? rec.t : t_max,rec);

	return lefthit || righthit;



}
bool bvh_node::bounding_box(double t0, double t1, aabb& out_box)const{

	out_box = box;
	return true;
}


#endif
