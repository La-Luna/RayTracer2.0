#ifndef MOVING_SPHERE
#define MOVING_SPHERE
#include "hitable.h"
#include "material.h"
class moving_sphere :public hitable{
public:
	moving_sphere(){};
	moving_sphere(points3 c0,points3 c1,double t0,double t1,double r,shared_ptr<material> m){
		center1 = c0;
		center2 = c1;
		time1 = t0;
		time2 = t1;
		radius = r;
		mat_ptr = m;
	};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& out_box)const override;
	points3 center(const double& t)const;


public:
	points3 center1;
	points3 center2;
	double radius;
	double time1, time2;
	shared_ptr<material> mat_ptr;

};

points3 moving_sphere::center(const double& t)const{
	points3 ans = ((center2 - center1) / (time2 - time1))*(t - time1) + center1;
	return ans;
}
bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec)const{

	vec3 A_C = r.A - center(r.tm);
	vec3 B = r.direction();
	vec3 A = r.origin();
	float a = dot(B, B);
	float b = dot(B, A_C);
	float c = dot(A_C, A_C) - radius*radius;
	float delta = b*b - a*c;
	if (delta > 0){
		float temp = (-b - sqrt(delta)) / a;
		if (temp<t_max&&temp>t_min){
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			vec3 outward_normal = (rec.p - center(r.tm)) / radius;
			rec.set_face_normal(r, outward_normal);
			//rec.normal = outward_normal;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(delta)) / a;
		if (temp<t_max&&temp>t_min){
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			vec3 outward_normal = (rec.p - center(r.tm)) / radius;
			//rec.normal = outward_normal;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}

	}
	//if(delta>0)std::cout << delta << std::endl;
	return false;
}

bool moving_sphere::bounding_box(double t0, double t1, aabb& out_box)const{
	aabb box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
	aabb box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
	out_box = surrounding_box(box0, box1);
	return true;


}


#endif
