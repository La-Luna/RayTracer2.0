#ifndef AABB_H
#define AABB_H
#include "vec3.h"
#include "rtweekend.h"
#include <iostream>
class aabb{
public:
	aabb(){};
	aabb(const points3& a, const points3& b) :_min(a), _max(b){};
	points3 min(){ return _min; }
	points3 max(){ return _max; }
	bool hit(const ray&r,double tmin,double tmax)const{
		for (int a = 0; a < 3; a++){
			auto t0 = fmin((_min[a] - r.A[a]) / r.B[a], (_max[a] - r.A[a]) / r.B[a]);
			auto t1 = fmax((_min[a] - r.A[a]) / r.B[a], (_max[a] - r.A[a]) / r.B[a]);

			tmin = max(t0, tmin);
			tmax = min(t1, tmax);
			if (tmin > tmax)return false;
		}
		return true;

	}





	points3 _min;
	points3 _max;
};

aabb surrounding_box(const aabb& a, const aabb& b){
	points3 msmall(fmin(a._min[0], b._min[0]),
		fmin(a._min[1], b._min[1]),
		fmin(a._min[2], b._min[2])
		);

	points3 mbig(fmax(a._max[0], b._max[0]),
		fmax(a._max[1], b._max[1]),
		fmax(a._max[2], b._max[2])
		);
	return aabb(msmall, mbig);
}

#endif
