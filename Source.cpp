#include <iostream>
#include "time.h"
#include"rtweekend.h"
#include"hitable_list.h"
#include "sphere.h"
#include "vec3.h"
#include "camera.h"
#include"color.h"
#include "material.h"
#include "moving_sphere.h"
#include "bvh.h"
#include "perlin.h"

color ray_color(ray& r,const hitable& world,int depth){ 
	hit_record rec;
	if (depth <= 0)return color(0.0, 0.0, 0.0);
	if (world.hit(r,0.001,infinity,rec)){
		//return color(1.0, 0.0, 0.0);

		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation*ray_color(scattered, world, depth - 1);
		return vec3(0.0, 0.0, 0.0);
	}
	else{
		vec3 unit_dir = unit_vector(r.direction());
		auto t = 0.5*(unit_dir.e[1] + 1.0);
		return (1.0 - t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
	}

}
hitable_list two_spheres(){
	hitable_list ans;
	shared_ptr<checker_texture> check = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	ans.add(make_shared<sphere>(points3(0.0, -10.0, 0.0), 10,make_shared < lambertian >(check)));
	ans.add(make_shared<sphere>(points3(0.0, 10.0, 0.0), 10,make_shared<lambertian>(check)));
	return ans;

}
hitable_list two_perlin_spheres(){
	hitable_list ans;
	shared_ptr<noise_texture> pertext = make_shared<noise_texture>(4.0);
	ans.add(make_shared<sphere>(points3(0.0, -1000.0, 0.0), 1000.0, make_shared<lambertian>(pertext)));
	ans.add(make_shared<sphere>(points3(0.0,2.0,0.0), 2, make_shared<lambertian>(pertext)));

	return ans;

}
hitable_list earth(){
	auto earthtex = make_shared<image_texture>("earthmap.jpg");
	auto earthsurf = make_shared<lambertian>(earthtex);
	auto globe = make_shared<sphere>(points3(0.0, 0.0, 0.0), 2, earthsurf);

	return hitable_list(globe);

}
hitable_list random_scene() {
	hitable_list world;
	auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	auto ground_material = make_shared<lambertian>(checker);
	world.add(make_shared<sphere>(points3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			points3 center(a + (float)0.9*random_double(), 0.2, b + (float)0.9*random_double());

			if ((center - points3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = vec3(random_double(), random_double(), random_double());
					sphere_material = make_shared<lambertian>(albedo);
					points3 center2 = center + vec3(0, random_double(0.0, 0.5), 0.0);
					world.add(make_shared<moving_sphere>(center,center2,0.0,1.0, 0.2, sphere_material));
				}
				 else if (choose_mat < 0.95) {
					// metal
					auto albedo = vec3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dieletric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dieletric>(1.5);
	world.add(make_shared<sphere>(points3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(points3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(points3(4, 1, 0), 1.0, material3));

	//return world;
	return hitable_list(make_shared<bvh_node>(world, 0.0, 1.0));
}

int main(){

	//image
	float aspect_ratio = 16.0 / 9.0;
	const int image_width = 900;
	const int image_height =static_cast<int> (image_width/aspect_ratio);
	const int samples_per_pixel = 150;
	const int max_depth = 50;

	//world



	//auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	//auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	//auto material_left = make_shared<dieletric>(1.5);
	//auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

	//auto material_left = make_shared <lambertian>(color(0.0, 1.0,0.0 ));
	//auto material_right = make_shared<lambertian>(color(1.0, 0.0, 0.0));

	//world.add(make_shared<sphere>(points3(0.0, 0.0, -1.0), 0.5, material_center));
	//world.add(make_shared<sphere>(points3(0.0, -100.5, -1.0), 100.0, material_ground));
	//world.add(make_shared<sphere>(points3(-1.0, 0.0, -1.0), 0.5, material_left));
	//world.add(make_shared<sphere>(points3(-1.0, 0.0, -1.0), -0.4, material_left));
	//world.add(make_shared<sphere>(points3(1.0, 0.0, -1.0), 0.5, material_right));

	//world.add(make_shared<sphere>(points3(-1.0, 0.0, -1.0), 1.0, material_left));
	//world.add(make_shared<sphere>(points3(1.0, 0.0, -1.0), 1.0, material_right));
	//camera


	hitable_list world;
	points3 lookfrom;
	points3 lookat;
	auto vfov=40.0;
	double aperture=0.0;


	switch(4){
	case 1:{
	world = random_scene();
	lookfrom=points3(13.0, 2.0, 3.0);
	lookat=points3(0.0, 0.0, 0.0);
	aperture = 0.1;
	vfov = 20.0;
	break;
	}
	case 2:{
		world = two_spheres();
		lookfrom = points3(13.0, 2.0, 3.0);
		lookat = points3(0.0, 0.0, 0.0);
		vfov = 20.0;
		break;
	}
	case 3:{
		world = two_perlin_spheres();
		lookfrom = points3(13.0, 2.0, 3.0);
		lookat = points3(0.0, 0.0, 0.0);
		vfov = 20.0;
		break;
	}
	case 4:{
		world = earth();
		lookfrom = points3(13.0, 2.0, 3.0);
		lookat = points3(0.0, 0.0, 0.0);
		vfov = 20.0;
		break;
	}

	}





	vec3 vup(0.0,1.0,0.0);
	double focus_disk=10;

	camera cam(lookfrom,lookat,vup,vfov,aspect_ratio,aperture,focus_disk,0.0,1.0);


	//render
	std::cout << "P3" << std::endl;
	std::cout << image_width << " " << image_height << std::endl;
	std::cout << "255" << std::endl;

	for (int j = image_height - 1; j >= 0; j--){
		std::cerr << "\rScaning remains:" << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++){
			color pixel_c(0.0, 0.0, 0.0);
			for (int s = 0; s < samples_per_pixel; s++){

				float u = (i+random_double()) / float(image_width - 1);
				float v = (j +random_double()) / float(image_height - 1);
				//std::cout << u << " " << v << std::endl;
				ray r = cam.get_ray(u, v);
				pixel_c += ray_color(r, world,max_depth);
			}
			write_color(std::cout, pixel_c,samples_per_pixel);

		}
	}
	std::cerr << "\nDone.\n";
	double finishtime = (double)(clock()/CLOCKS_PER_SEC);
	std::cerr << "RunTime:" << finishtime << std::endl;

}