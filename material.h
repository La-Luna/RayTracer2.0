#ifndef MATERIAL_H
#define MATERIAL_H
#include "hitable.h"
#include "ray.h"
#include "vec3.h"
#include"texture.h"
#include <iostream>
//class material;
//struct hit_record{
//	float t;
//	vec3 p;
//	vec3 normal;
//	material* mat_ptr;
//};
//struct hit_record{
//	double t;
//	vec3 p;
//	vec3 normal;
//	bool front_face;
//	shared_ptr<material> mat_ptr;
//	//material* mat_ptr;
//	inline void set_face_normal(const ray&r, const vec3& outward_normal){
//		this->front_face = dot(r.direction(), outward_normal) < 0;
//		this->normal = front_face ? outward_normal : (-outward_normal);
//
//	}
//
//
//};
struct hit_record;
vec3 reflect(const vec3& rayin,const vec3& n){
	return rayin - 2 * dot(rayin, n)*n;

}
double schlick(double cos_theta, double ref_idx){
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0*r0;
	return r0 + (1 - r0)*pow((1 - cos_theta), 5);
}




class material{
public:
	virtual bool scatter( ray& r_in, hit_record& rec, vec3& attenuation, ray& scattered)const=0;
};

class lambertian :public material{
public:
	lambertian(const color& a):albedo(make_shared<solid_color>(a)){}
	lambertian(shared_ptr<texture> a) :albedo(a){};
	virtual bool scatter( ray& r_in, hit_record& rec, vec3& attenuation,  ray& scattered)const{ 
		vec3 target_s = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target_s - rec.p,r_in.time());
		attenuation = albedo->value(rec.u,rec.v,rec.p);
		return true;

	};


	//vec3 albedo;
	shared_ptr<texture> albedo;
};

class metal :public material{
public:
	metal(const vec3& a, float f) :albedo(a){ if (f < 1.0)fuzz = f; else fuzz = 1.0; }
	virtual bool scatter(ray& r_in, hit_record& rec, vec3& attenuation, ray& scattered)const{
		vec3 reflectray = reflect(unit_vector(r_in.direction()), rec.normal);

		attenuation = albedo;
		scattered = ray(rec.p, reflectray+fuzz*random_in_unit_sphere());
		return (dot(scattered.direction(), rec.normal) > 0);
	}


	vec3 albedo;
	float fuzz;
};

class dieletric :public material{
public:

	dieletric(double ref) :ref_idex(ref){};
	virtual bool scatter(ray& r_in, hit_record& rec, vec3& attenuation, ray& scattered)const override{
		attenuation = vec3(1.0, 1.0, 1.0);
		double etai_over_etat = rec.front_face ? 1.0 / ref_idex : ref_idex;

		vec3 unit_direction = unit_vector(r_in.direction());

		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
		if (etai_over_etat*sin_theta > 1.0){
			vec3 reflectedray = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p,reflectedray);
			return true;
		}

		if (random_double() < schlick(cos_theta,etai_over_etat)){
			vec3 reflectedray = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p,reflectedray);
			return true;
		}

		vec3 refractedray = refract(unit_direction, rec.normal,etai_over_etat);
		scattered = ray(rec.p, refractedray);
		return true;
	}


		double ref_idex;
};

#endif
