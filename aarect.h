#ifndef _AARECT_H
#define _AARECT_H
#include "hitable.h"
#include "material.h"
#include "aabb.h"

class xy_rect:public hitable
{
public:
	xy_rect(){};
	xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> _m){
		x0 = _x0;
		x1 = _x1;
		y0 = _y0;
		y1 = _y1;
		k = _k;
		mp = _m;
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const override{
		out_box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
		return true;
	}


protected:
public:

	double x0;
	double x1;
	double y0;
	double y1;
	double k;
	shared_ptr<material> mp;
};
bool xy_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec)const{
	double t = (k - r.origin().z()) / r.direction().z();
	if (t<t_min || t>t_max)return false;
	double x = r.origin().x() + r.direction().x()*t;
	double y = r.origin().y() + r.direction().y()*t;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y-y0) / (y1 - y0);
	vec3 outward_normal= vec3(0.0, 0.0, 1.0);
	rec.set_face_normal(r, outward_normal);
	rec.p = r.point_at_parameter(t);
	rec.mat_ptr = mp;
	rec.t = t;
	return true;
}


class xz_rect :public hitable{
public:
	xz_rect() {}
	xz_rect(double _x0, double _x1, double _z0, double _z1, double _k,
		shared_ptr<material> mat)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const override;

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		output_box = aabb(points3(x0, k - 0.0001, z0), points3(x1, k + 0.0001, z1));
		return true;
	}

public:
	shared_ptr<material> mp;
	double x0, x1, z0, z1, k;

};
class yz_rect :public hitable{
public:
	yz_rect() {}
	yz_rect(double _y0, double _y1, double _z0, double _z1, double _k,
		shared_ptr<material> mat)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const override;

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		output_box = aabb(points3(k - 0.0001, y0, z0), points3(k + 0.0001, y1, z1));
		return true;
	}

public:
	shared_ptr<material> mp;
	double y0, y1, z0, z1, k;
};
bool xz_rect::hit(const ray& r, float t0, float t1, hit_record& rec) const {
	auto t = (k - r.origin().y()) / r.direction().y();
	if (t < t0 || t > t1)
		return false;
	auto x = r.origin().x() + t*r.direction().x();
	auto z = r.origin().z() + t*r.direction().z();
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	auto outward_normal = vec3(0, 1, 0);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	return true;
}

bool yz_rect::hit(const ray& r, float t0, float t1, hit_record& rec) const {
	auto t = (k - r.origin().x()) / r.direction().x();
	if (t < t0 || t > t1)
		return false;
	auto y = r.origin().y() + t*r.direction().y();
	auto z = r.origin().z() + t*r.direction().z();
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;
	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	auto outward_normal = vec3(1, 0, 0);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	return true;
}
#endif
