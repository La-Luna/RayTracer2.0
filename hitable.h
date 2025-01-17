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
		  offset=off;
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

class rotate_y :public hitable{
public:
	rotate_y(){};
	rotate_y(shared_ptr<hitable> p, double theta) ;
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const override;

public:
	shared_ptr<hitable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	aabb  bbox;
};
rotate_y::rotate_y(shared_ptr<hitable> p, double angle):ptr(p){
	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);

	points3 my_min(infinity, infinity, infinity);
	points3 my_max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				auto x = i*bbox.max().x() + (1 - i)*bbox.min().x();
				auto y = j*bbox.max().y() + (1 - j)*bbox.min().y();
				auto z = k*bbox.max().z() + (1 - k)*bbox.min().z();

				auto newx = cos_theta*x + sin_theta*z;
				auto newz = -sin_theta*x + cos_theta*z;

				vec3 tester(newx, y, newz);

				for (int c = 0; c < 3; c++) {
					my_min[c] = fmin(my_min[c], tester[c]);
					my_max[c] = fmax(my_max[c], tester[c]);
				}
			}
		}
	}

	bbox = aabb(my_min, my_max);
}
bool rotate_y::hit(const ray& r, float t_min, float t_max, hit_record& rec)const{
	auto origin = r.origin();
	auto direction = r.direction();

	origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
	origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];

	direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
	direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

	ray rotated_r(origin, direction, r.time());

	if (!ptr->hit(rotated_r, t_min, t_max, rec))
		return false;

	auto p = rec.p;
	auto normal = rec.normal;

	p[0] = cos_theta*rec.p[0] + sin_theta*rec.p[2];
	p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];

	normal[0] = cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
	normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

	rec.p = p;
	rec.set_face_normal(rotated_r, normal);

	return true;
}
bool rotate_y::bounding_box(double t0, double t1, aabb& out_box)const{
	out_box = bbox;
	return hasbox;
}
#endif

