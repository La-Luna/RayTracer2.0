#ifndef TEXTURE_H
#define TEXTURE_H
#include "rtweekend.h"
#include "vec3.h"
#include "perlin.h"
#include "rtw_stb_image.h"
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

class noise_texture :public texture{
public:
	noise_texture(double s):scale(s){};
	virtual color value(double u, double v, const points3& p)const override{
		//return color(1.0, 1.0, 1.0)*0.5*(1.0+noise.noise(p*scale));
		return color(1.0, 1.0, 1.0)*0.5*(1+sin(scale*p.z())+10*noise.turb(p));
	}


public:
	perlin noise;
	double scale;
};

class image_texture :public texture{
public:
	const static int bytes_per_pixel = 3;
	image_texture() :data(nullptr), width(0), height(0), bytes_per_scanline(0){};
	image_texture(const char* filename){
		auto components_per_pixel = bytes_per_pixel;
		data = stbi_load(filename, &width, &height, &components_per_pixel,components_per_pixel);
		if (!data){
			std::cerr << "error:cannot read image file called" << filename << ".\n";
			width = height = 0;
		}
		bytes_per_scanline = bytes_per_pixel*width;


	}
	~image_texture(){
		delete data;
	}
	virtual color value(double u, double v, const points3& p)const override;
	 
private:
	unsigned char* data;
	int width, height;
	int bytes_per_scanline;
};

color image_texture::value(double u, double v, const points3& p)const{
	if (data == nullptr){
		return color(0.0, 1.0, 0.0);
	}
	u = clamp(u, 0.0, 1.0);
	v = clamp(v, 0.0, 1.0);

	auto i = static_cast<int>(u*width);
	auto j = static_cast<int>(v*height);

	if (i >= width)i = width - 1;
	if (j >= height)j = height - 1;

	const auto color_scale = 1.0 / 255.0;
	auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

	return color(pixel[0] * color_scale, pixel[1] * color_scale, pixel[2] * color_scale);

}

#endif