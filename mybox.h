#ifndef BOX_H
#define BOX_H
#include"hitable.h"
#include"rtweekend.h"
#include"aarect.h"
#include"hitable_list.h"

class box :public hitable{
public:
	box(){};
	box(const points3& min, const points3& max, shared_ptr<material> ptr);
	
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const override;


public:
	points3 box_min;
	points3 box_max;
	hitable_list sides;


};

box::box(const points3& p0, const points3& p1, shared_ptr<material> ptr){
	box_min = p0;
	box_max = p1;
	sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
	sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

	sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
	sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

	sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
	sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}
bool box::hit(const ray& r, float t_min, float t_max, hit_record& rec)const{

	return  sides.hit(r, t_min, t_max, rec);

}
bool box::bounding_box(double t0, double t1, aabb& out_box)const{
	out_box = aabb(box_min, box_max);
	return true;
}

#endif