#include "quaternion.h"

quaternion::quaternion(){
	real=1;
	x=y=z=0;
	halfTheta=0;
}

quaternion::quaternion(float _r, float _i, float _j, float _k){
	real=_r;
	x=_i;
	y=_j;
	z=_k;
	halfTheta=0;
}

quaternion::quaternion (glm::vec3 axis, float theta){
	halfTheta=theta/2.0f;
	real=axis[0]*cosf(theta/2.0f);
	x=axis[0]*sinf(theta/2.0f);
	y=axis[1]*sinf(theta/2.0f);
	z=axis[2]*sinf(theta/2.0f);
}
	
quaternion::~quaternion(){

}
	
float quaternion::length() const{
	float len=sqrt(pow(real,2)+pow(x,2)+pow(y,2)+pow(z,2));
	return len;
}

float quaternion:: normalize(){
	return 0;
}

float quaternion::operator[](unsigned int index) const{
	assert(index>=0&&index<=3);
	if(index==0)
		return real;
	else if(index==1)
		return x;
	else if(index==2)
		return y;
	else
		return z;
}
	
float quaternion:: operator *(const quaternion& q2){
	return 0;
}

ostream &operator<<(ostream &stream, quaternion q){
	stream<< "< " <<q.real << ", " <<q.x <<", " <<q.y <<", "<<q.z<< ">" <<endl;
	return stream;
}

glm::mat4 quaternion:: getRotation(){
	glm::mat4 rotation=glm::mat4(1-2*(pow(y,2))-2*(pow(z,2)), 2*x*y-2*real*z, 2*x*z+2*real*y,0,
						2*x*y+2*real*z,1-2*(pow(x,2))-2*(pow(z,2)), 2*y*z+2*real*x,0,
						2*x*z-2*real*y,2*y*z-2*real*x, 1-2*(pow(x,2))-2*(pow(y,2)),0,
						0,0,0,1);

	return rotation;
}

void quaternion::setTheta(float angle){
	halfTheta=angle/2.0f;
}

void quaternion::setAxis(glm::vec3 axis){
	real=axis[0]*cosf(halfTheta);
	x=axis[0]*sinf(halfTheta);
	y=axis[1]*sinf(halfTheta);
	z=axis[2]*sinf(halfTheta);
}