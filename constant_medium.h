#ifndef _CONSTANT_MEDIUM_H
#define _CONSTANT_MEDIUM_H
#include"hitable.h"
#include "material.h"
#include "texture.h"
class constant_medium :public hitable{
public:
	constant_medium(){}
	constant_medium(shared_ptr<hitable> b, double d, shared_ptr<texture> a) :boundary(b), neg_inv_density(d), phase_function(make_shared<isotropic>(a)){}
	constant_medium(shared_ptr<hitable> b, double d, color c) :boundary(b), neg_inv_density(d), phase_function(make_shared<isotropic>(c)){}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const override{ return boundary->bounding_box(t0, t1, out_box); };

public:
	shared_ptr<hitable> boundary;
	shared_ptr<material> phase_function;
	double neg_inv_density;
};
bool constant_medium::hit(const ray& r, float t_min, float t_max, hit_record& rec)const{
	hit_record rec1, rec2;
	if (!boundary->hit(r, t_min, t_max, rec1))return false;
	if (!boundary->hit(r, t_min, t_max, rec2))return false;

	if (rec1.t < t_min)rec1.t = t_min;
	if (rec1.t > t_max)rec1.t = t_max;

	if (rec1.t > rec2.t)return false;
	if (rec1.t < 0)rec1.t = 0;

	const auto ray_length = r.direction().length();
	const auto diatance_inside_boundary = (rec2.t - rec1.t)*ray_length;

	const auto hit_distance = neg_inv_density*log(random_double());
	if (hit_distance > diatance_inside_boundary)return false;

	rec.t = rec1.t + hit_distance / ray_length;
	rec.p = r.point_at_parameter(rec.t);

	rec.normal = vec3(1.0, 0.0, 0.0);
	rec.front_face = true;
	rec.mat_ptr = phase_function;

	return true;
}


#endif
