#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>
#include <assert.h>
#include "../glm/glm.hpp"
using namespace std;

class quaternion {

public:
	quaternion();
	quaternion(float, float, float, float);
	quaternion (glm::vec3, float);
	~quaternion();
	
	float length() const;
	float normalize();

	float operator[](unsigned int index) const;
	float operator *(const quaternion& q2);
	friend ostream &operator<<(ostream &stream, quaternion q);
	glm::mat4 getRotation();
	void setTheta(float angle);
	void setAxis(glm::vec3 axis);

	float halfTheta;	//in radians
	float real;
	float x;
	float y;
	float z;

};
#endif