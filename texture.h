#ifndef TEXTURE_H
#define TEXTURE_H
#include "rtweekend.h"
#include "vec3.h"
class texture{
public:
	virtual color value(double u,double v,const points3& p)const = 0;

};

class solid_color :public texture{
public:
	solid_color(double red, double green, double blue){ color_value = color(red, green, blue); }
	solid_color(color c){ color_value = c; };
	solid_color(){};
	virtual color value(double u,double v,const points3& p)const override{
		return color_value;
	}


private:
	color color_value;
};

#endif