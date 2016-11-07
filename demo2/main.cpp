#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
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
		t1 = (-B - sqrt(discriminant))/(2*A);  // the closer solution of the intersection for ONE sphere
		*hit = (*ray).src + ((*ray).dir)*t1;
		return true;
	}
	else {                               // this is the ray from hit
		t1 = (-B - sqrt(discriminant))/(2*A);
		t2 = (-B + sqrt(discriminant))/(2*A);
		if (t1 > 0.0000005 || t2 > 0.000005) return true;
		else return false;
	}
}


int main(){
	const char * filename = "test.png";
	unsigned width = 640;
	unsigned height = 480;

	vector<unsigned char> image;
	image.resize(width*height*4);

	//create 3 spheres
	Sphere spheres[3];
	spheres[0] = (Sphere){Vector(600,400,225), 200};
	spheres[1] = (Sphere){Vector(320,240,610), 400};
	spheres[2] = (Sphere){Vector(-100,-100,500), 350};
	
	Color colors[3];
	colors[0] = (Color){217, 218, 52, 255};
	colors[1] = (Color){100,200,150,255};
	colors[2] = (Color){255,255, 255, 255};

	// create light
	Vector light(320, 240, 10);

	//create eye
	Vector eye(width/2, height/2, -100);

	
	Vector hit;       // the position of the intersection
	Vector closer_hit(0,0,(numeric_limits<int>::max)());
	
	Ray shadow_ray;   // the ray from the hit to the light
	bool illuminated = false;
	Vector shadow_hit;  //Actually it's useless
	double distance;   //dsitance between light and hit 
	double scalar;     //scalar to mofify the brightness

	
	int which_object = -1;

	bool shadow = false;

	for (unsigned y=0; y<height; y++){
		for (unsigned x=0; x<width; x++){
			
			//write the primary ray
			Vector pixel(x, y, 10);
			Ray ray = {eye, (pixel - eye).normalize()};

			
			// analyze the primary ray
			for (int i=0;i<3;i++){
				if (intersect(&ray, &spheres[i], &hit)){
					if (hit.getZ() < closer_hit.getZ()) {
						closer_hit = hit; 
						which_object = i; 
					}
				}
			}

			// //analyze the shadow ray if the primary ray has intersections with spheres
			if (which_object != -1){
			
				shadow_ray.src = closer_hit;
				shadow_ray.dir = (light-closer_hit).normalize();
				for (int i = 0; i<3;i++){

					if (intersect(&shadow_ray, &spheres[i],&shadow_hit)){
						shadow = true; 
						break;
					}
				}
				if (!shadow) illuminated = true; 
				
			}

			//if illuminated, determine color and brightness
			if (illuminated){
				
				distance = (closer_hit - light).magnitude();	
				scalar = (200/distance)*(200/distance);

				image[4 * width * y + 4 * x + 0] = colors[which_object].red*scalar;
    			image[4 * width * y + 4 * x + 1] = colors[which_object].green*scalar;
    			image[4 * width * y + 4 * x + 2] = colors[which_object].blue*scalar;
    			image[4 * width * y + 4 * x + 3] = colors[which_object].alpha;
    			
			}

			else {
				image[4 * width * y + 4 * x + 0] = 0;
    			image[4 * width * y + 4 * x + 1] = 0;
    			image[4 * width * y + 4 * x + 2] = 0;
    			image[4 * width * y + 4 * x + 3] = 255;

			}

			closer_hit.setZ((double)(numeric_limits<int>::max)()); 
			which_object = -1;
			shadow = false;
			illuminated = false;

		}
	}

	//save the image into PNG format
	lodepng::encode(filename,image,width,height);

	return 0;
}



