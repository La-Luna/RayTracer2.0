#ifndef RAY_H
#define RAY_H
#include "vec3.h"
#include <iostream>


//  p(t)=A+B*t;
class ray{
public:
	ray(){};
	ray(const vec3& a, const  vec3& b, double time = 0.0){ A = a; B = b; tm = time; }
	vec3 origin()const { return A; }
	vec3 direction()const { return B; }
	vec3 point_at_parameter(float t) const{
		return A + t*B;
	}
	double time()const{ return tm; }


	vec3 A;
	vec3 B;
	double tm;
};
#endif

