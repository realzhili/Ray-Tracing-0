#ifndef _Vector_h
#define _Vector_h

#include <cmath>

class Vector {
private:
	double x;
	double y;
	double z;

public:
	Vector(){x = 0.0; y = 0.0; z = 0.0; }
	Vector(double a, double b, double c){x = a; y = b; z = c;}
	double getX(){return x;}
	double getY(){return y;}
	double getZ(){return z;}
	void setZ(double val){z = val;}

	double magnitude(){return sqrt(x*x+y*y+z*z);}

	Vector normalize(){
		double magnitude = sqrt(x*x+y*y+z*z);
		return Vector(x/magnitude, y/magnitude, z/magnitude);
	}

	Vector negative(){ return Vector(-x, -y, -z);}

	
	Vector crossProductWith(Vector a){
		return Vector(y*a.getZ()-z*a.getY(), z*a.getX()-x*a.getZ(), x*a.getY()-y*a.getX());
	}

	Vector operator+ (Vector a){
		return Vector(x+a.getX(), y+a.getY(), z+a.getZ());
	}

	Vector operator- (Vector a){
		return Vector(x-a.getX(), y-a.getY(), z-a.getZ());
	}

	//calculate the dot product
	double operator* (Vector a){
		return x*a.getX()+y*a.getY()+z*a.getZ();
	}

	Vector operator* (double scalar){
		return Vector(x*scalar, y*scalar, z*scalar);
	}


};




#endif
