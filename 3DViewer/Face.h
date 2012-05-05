#ifndef FACE_H
#define FACE_H

#include "Vertex.h"
#include "HalfEdge.h"
#include "../glm/glm.hpp"
#include <iostream>
using namespace std;

class HalfEdge;
class Vertex;

class Face {

public:
    //Constructors////////////////////////////
    Face(HalfEdge*, const float[]);
	Face(HalfEdge*, const float[], const int);
	Face(const float[],const int);
    //////////////////////////////////////////
    
    //Destructor
    ~Face(void);
    
    //Instance fields/////////////////////////
	HalfEdge* he; //This is arbitrarily any one of the half edges that comprise this face
	float color[3]; //RGB values from 0-255
	int id; //?
	int num; //Unique identifier
    //////////////////////////////////////////
    
    
    int verticesCount(void); //This is the number of vertices this face has
	glm::vec3 normal(void); //Returns the normal of this face
	bool checkFlat(); //Checks to make sure the face is planar?
	HalfEdge* checkPlanarity(); //Checks to make sure the face is planar?
	HalfEdge* split(); //Splits a face into two or more smaller faces
	glm::vec3 setCentroid(); //Returns the geometric center of this face
	Vertex* centroid;
	bool checkFace();
	int index;  //This is the index that the first vertex of this face will appear in the cbo, ibo, and vbo
};
#endif