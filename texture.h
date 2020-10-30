#ifndef TEXTURE_H
#define TEXTURE_H
#include "rtweekend.h"
#include "vec3.h"
class texture{
public:
	//texture(const color& temp){};
	virtual color value(double u,double v,const points3& p)const = 0;

};

class solid_color :public texture{
public:
	solid_color(double red, double green, double blue){ color_value = color(red, green, blue); }
	//solid_color(color c){ color_value = c; };
	solid_color(){};
	solid_color(const color&c){ color_value = c; }
	virtual color value(double u,double v,const points3& p)const override{
		return color_value;
	}


private:
	color color_value;
};

class checker_texture :public texture{
public:
	checker_texture(){};
	checker_texture(shared_ptr<texture>t0, shared_ptr<texture>t1) :odd(t1),
	even(t0){
		
	};
	checker_texture(color c1, color c2) : even(make_shared<solid_color>(c1)),
	odd(make_shared<solid_color>(c2)){

	}
	virtual color value(double u,double v,const points3& p)const{
		auto sines = sin(10 * p.e[0])*sin(10 * p.e[1])*sin(10 * p.e[2]);
		if (sines < 0){
			return odd->value(u, v, p);
		}
		else{
			return even->value(u, v, p);
		}

	}


public:
	shared_ptr<texture> odd;
	shared_ptr<texture> even;
};

#endif