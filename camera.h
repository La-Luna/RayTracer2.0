#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
class camera{
public:
	camera(
		vec3 lookfrom,
		vec3 lookat,
		vec3 vup,
		double vfov,
		double aspect_ratio,
		double aperture,
		double focus_disk,
		double t0=0.0,
		double t1=0.0
		){



		//float aspect_ratio = 16.0 / 9.0;
		double theta = degrees_to_radians(vfov);
		double h = tan(theta / 2.0);
		float viewport_height = 2.0*h;
		float viewport_width = viewport_height*aspect_ratio;
		//float facal_length = 1.0;


		 w = unit_vector(lookfrom - lookat);
		 u = unit_vector(cross(vup, w));
		 v = cross(w, u);


		eyepos = lookfrom;
		horizontal = focus_disk*viewport_width*u;
		vertical = focus_disk*viewport_height*v;
		left_down_corner = eyepos - horizontal / 2.0 - vertical / 2.0 - focus_disk*w;

		lens_radius = aperture / 2.0;
		time0 = t0;
		time1 = t1;
	};
	ray get_ray(float s, float t){

		vec3 r0 = lens_radius*random_in_unit_disk();
		vec3 offeset =  r0[0]*u  + r0[1]*v;

		vec3 direction = left_down_corner + s*horizontal + t*vertical - eyepos-offeset;
		return ray(eyepos+offeset, direction,random_double(time0,time1)); 
	}



	vec3 left_down_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 eyepos;
	vec3 u, v, w;
	double lens_radius;
	double time0;
	double time1;
};

#endif // !CAMERA_H
