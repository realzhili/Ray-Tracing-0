#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "lodepng.h"

#include "Vector.h"

using namespace std;

struct Ray{
	Vector src;
	Vector dir;    //normalized 
};

struct Sphere{
	Vector center;
	double radius;
};

struct Color{
	unsigned red;
	unsigned green;
	unsigned blue;
	unsigned alpha;
};

bool intersect (Ray *ray, Sphere *sphere, Vector *hit);


int main(){
	const char * filename = "test.png";
	unsigned width = 640;
	unsigned height = 480;

	vector<unsigned char> image;
	image.resize(width*height*4);

	
	
	//create a sphere
	Vector center(width/2, height/2, 500);
	double radius = 400;
	Sphere sphere = {center, radius};
	Color sphere_color = {217, 218, 52, 255};



	// create light
	Vector light(320, 240, 0);

	//create eye
	Vector eye(width/2, height/2, -100);

	//
	Vector hit;       // the position of the intersection
	Ray shadow_ray;   // the ray from the hit to the light
	bool illuminated = false;
	Vector shadow_hit;  //Actually it's useless
	double distance;   //dsitance between light and hit 
	double scalar;     //scalar to mofify the brightness


	for (unsigned y=0; y<height; y++){
		for (unsigned x=0; x<width; x++){
			
			//write the primary ray
			Vector pixel(x, y, 10);
			Ray ray = {eye, (pixel - eye).normalize()};
			
			//if intersect, determine shadow or not
			if (intersect(&ray, &sphere, &hit)){

				shadow_ray.src = hit;
				shadow_ray.dir = (light-hit).normalize();
				
				if (!intersect(&shadow_ray, &sphere,&shadow_hit )){
					illuminated = true;
				}
			}

			//if illuminated, determine color and brightness
			if (illuminated){
				distance = (hit - light).magnitude();
				scalar = (100/distance)*(100/distance);

				image[4 * width * y + 4 * x + 0] = 217*scalar;
    			image[4 * width * y + 4 * x + 1] = 218*scalar;
    			image[4 * width * y + 4 * x + 2] = 82*scalar;
    			image[4 * width * y + 4 * x + 3] = 255;
    			illuminated = false;

			}

			else {
				image[4 * width * y + 4 * x + 0] = 0;
    			image[4 * width * y + 4 * x + 1] = 0;
    			image[4 * width * y + 4 * x + 2] = 0;
    			image[4 * width * y + 4 * x + 3] = 255;

			}
		}
	}

	//save the image into PNG format
	lodepng::encode(filename,image,width,height);

	return 0;
}



//the calculation to figure out whether a ray intersects with the object or not
bool intersect (Ray *ray, Sphere *sphere, Vector *hit){
	double A = 1.0;
	double B, C;
	double t1, t2; // the solution to the equation
	// we can write ray as R0+t*Rd
	//for R0
	double x0 = (*ray).src.getX();
	double y0 = (*ray).src.getY();
	double z0 = (*ray).src.getZ();
	//for Rd
	double xd = (*ray).dir.getX();
	double yd = (*ray).dir.getY();
	double zd = (*ray).dir.getZ();
	//for center of sphere
	double xc = (*sphere).center.getX();
	double yc = (*sphere).center.getY();
	double zc = (*sphere).center.getZ();
	//for radius of sphere 
	double sr = (*sphere).radius;

	B = 2*(xd*(x0-xc)+yd*(y0-yc)+zd*(z0-zc));
	C = (x0-xc)*(x0-xc) + (y0-yc)*(y0-yc) + (z0-zc)*(z0-zc) - sr*sr;

	double discriminant = B*B - 4*A*C;

	if (discriminant < 0) return false;
	else if ((*ray).src.getZ() < 10){    //this is the ray from eye
		t1 = (-B - sqrt(discriminant))/(2*A);
		*hit = (*ray).src + ((*ray).dir)*t1;
		return true;
	}
	else {                               // this is the ray from hit
		t1 = (-B - sqrt(discriminant))/(2*A);
		t2 = (-B + sqrt(discriminant))/(2*A);
		if (t1 > 0.0005 || t2 > 0.0005) return true;
		else return false;
	}


	
}