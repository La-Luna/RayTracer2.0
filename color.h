#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3.h"
#include "rtweekend.h"
#include "math.h"
void write_color(std::ostream & out, color c,int samples_per_pixels){
	double scale = 1.0 / double(samples_per_pixels);
	color pixel_color_temp = scale*c;
	color pixel_color = vec3(sqrt(pixel_color_temp[0]), sqrt(pixel_color_temp[1]), sqrt(pixel_color_temp[2]));

	out<<static_cast<int>(255.9*clamp(pixel_color[0],0.0,0.999))<<' '
		<< static_cast<int>(255.9*clamp(pixel_color[1], 0.0, 0.999)) << ' '
		<< static_cast<int>(255.9*clamp(pixel_color[2], 0.0, 0.999)) << '\n';
}

#endif
