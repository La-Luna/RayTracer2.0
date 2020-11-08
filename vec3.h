#ifndef VEC3_H
#define VEC3_H
#include<math.h>
#include<iostream>
#include "rtweekend.h"

double random_double(double min, double max);
class vec3{
public:
	vec3(){};
	vec3(const vec3& temp){
		e[0] = temp.e[0];
		e[1] = temp.e[1];
		e[2] = temp.e[2];
	}
	vec3(float a, float b, float c){
		e[0] = a;
		e[1] = b;
		e[2] = c;
	}
	inline float operator [](int i)const{
		return e[i];
	}
	inline float& operator[](int i){
		return e[i];
	}
	inline float length()const{
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline float squared_length()const{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline vec3& operator +=(const vec3& temp);
	vec3 operator -()const{ return vec3(-e[0], -e[1], -e[2]); }

	inline double x()const{
		return e[0];
	}
	inline double y()const{
		return e[1];
	}
	inline double z()const{
		return e[2];
	}



public:
	inline static vec3 random(double a, double b){
		return vec3(random_double(a, b), random_double(a, b), random_double(a, b));
	}

	float e[3];
};

using color = vec3;
using points3 = vec3;


inline vec3 operator* (float k, const vec3& temp){
	return vec3(k*temp[0], k*temp[1], k*temp[2]);
}
inline vec3 operator+ (const vec3& a, const  vec3& b){
	return vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
inline vec3 operator/(const vec3& temp, float k){
	if (k != 0){
		return vec3(temp[0] / k, temp[1] / k, temp[2] / k);
	}

}
inline vec3 unit_vector(const vec3& temp){
	//float len = sqrt(temp[0] * temp[0] + temp[1] * temp[1] + temp[2] * temp[2]);
	return temp/temp.length();
}
inline float dot(const vec3& a,const vec3&b){
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
inline vec3 operator-(const vec3 & a, const vec3&b)
{
	return vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

inline vec3 operator+(const vec3&temp, float k){
	return vec3(temp[0] + k, temp[1] + k, temp[2] + k);
}
inline vec3& vec3::operator +=(const vec3& temp){
	e[0] += temp[0];
	e[1] += temp[1];
	e[2] += temp[2];
	return *this;
}
inline vec3 operator*(const vec3& a, const vec3& b){
	return vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

vec3 refract(const vec3& uv,const vec3 &n,double etai_over_etat){
	double cos_theta = dot(-uv, n);
	vec3 r_out_perp=etai_over_etat*(uv+cos_theta*n);
	vec3 r_out_parrallel=-sqrt(fabs(1.0-r_out_perp.squared_length()))*n;
	vec3 r = r_out_perp + r_out_parrallel;
	return r;

}
vec3 cross(const vec3& u, const vec3&v){
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
vec3 operator*(const vec3& temp, double k){
	return vec3(temp[0] * k, temp[1] * k, temp[2] * k);
}

#endif


