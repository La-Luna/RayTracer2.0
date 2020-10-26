#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <cstdlib>
// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

// Common Headers

#include "ray.h"
#include "vec3.h"



//random
double random_double(){

	//[0,1)
	double ans = (double)rand() / (double)(RAND_MAX + 1);
	return ans;
}

double random_double(double min, double max){
	double ans = min + (max - min)*random_double();
	return ans;
}

vec3 random(){
	return vec3(random_double(), random_double(), random_double());
}
int random_int(int a,int b){
	return static_cast<int>(random_double(a,b+1));

}
vec3 random(double min, double max){
	return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}
vec3 random_in_unit_sphere(){
	while (true){
		vec3 p = random(-1.0, 1.0);
		if (p.length() < 1.0)return p;
	}
}
vec3 random_in_unit_disk(){
	while (true){
		vec3 p = vec3(random_double(-1.0, 1.0), random_double(-1.0, 1.0), random_double(-1.0, 1.0));
		if (p.squared_length() < 1.0)return p;
	}
}
double clamp(double x, double min, double max){
	if (x < min)return min;
	if (x > max)return max;
	return x;
}


#endif
