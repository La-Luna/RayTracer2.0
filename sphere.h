#ifndef SPHERE_H
#define SPHERE_H
#include "hitable.h"
#include "ray.h"
#include "material.h"
#include "vec3.h"
class sphere :public hitable{
public:
	sphere(){};
	sphere(vec3 cen, float r, shared_ptr<material> m){ center = cen; radius = r; mat_ptr = m; }
	//sphere(vec3 cen, float r){ center = cen; radius = r;}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const override;
public:
	float radius;
	vec3 center;
	shared_ptr<material> mat_ptr;
	//material* mat;
};
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec)const {
	vec3 A_C = r.A - center;
	vec3 B = r.direction();
	vec3 A = r.origin();
	float a = dot(B, B);
	float b = dot(B, A_C);
	float c = dot(A_C, A_C) - radius*radius;
	float delta = b*b -  a*c;
	if (delta > 0){
		float temp = (-b - sqrt(delta)) /a;
		if (temp<t_max&&temp>t_min){
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			vec3 outward_normal = (rec.p - center)/radius;
			rec.set_face_normal(r, outward_normal);
			//rec.normal = outward_normal;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(delta)) /a;
		if (temp<t_max&&temp>t_min){
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			vec3 outward_normal = (rec.p - center) / radius;
			//rec.normal = outward_normal;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}

	}
	//if(delta>0)std::cout << delta << std::endl;
	return false;

}
bool sphere::bounding_box(double t0, double t1, aabb& out_box)const{
	out_box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}

#endif



