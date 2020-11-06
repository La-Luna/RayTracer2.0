#ifndef HITABLE_H
#define HITABLE_H
#include "vec3.h"
#include "ray.h"
#include "aabb.h"
//#include "material.h"
class material;
struct hit_record{
	double t;
	double u;
	double v;
	vec3 p;
	vec3 normal;
	bool front_face;
	shared_ptr<material> mat_ptr;
	//material* mat_ptr;
	inline void set_face_normal(const ray&r, const vec3& outward_normal){
		this->front_face = dot(r.direction(), outward_normal) < 0;
		this->normal = front_face? outward_normal : (-outward_normal);

	}


};


class hitable{
public:
	//virtual bool hit(const ray& r,float t_min,float t_max,hit_record& rec)const = 0;
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const = 0;

};



class translate:public hitable{
public:
	translate(){};
	translate(shared_ptr<hitable> p, const vec3& off){
		ptr = p;
		off = offset;
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const override;

public:
	shared_ptr<hitable> ptr;
	vec3 offset;

};

bool translate::hit(const ray& r, float t_min, float t_max, hit_record& rec)const{
	ray moved_r(r.origin() - offset, r.direction(), r.time());
	if (!ptr->hit(r, t_min, t_max, rec))return false;
	rec.p += offset;
	rec.set_face_normal(moved_r, rec.normal);
}

bool translate::bounding_box(double t0, double t1, aabb& out_box)const{
	if (!ptr->bounding_box(t0, t1, out_box))return false;
	out_box = aabb(out_box.min() + offset, out_box.max() + offset);
	return true;
}
#endif

