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
	HalfEdge* he;
	float color[3];
	int id;
	int num;
	Face(HalfEdge*, const float[]);
	Face(HalfEdge*, const float[], const int);
	Face(const float[],const int);
	~Face(void);
	int verticesCount(void);
	glm::vec3 normal(void);
	bool checkFlat();
	HalfEdge* checkPlanarity();
	HalfEdge* split();
	glm::vec3 setCentroid();
	Vertex* centroid;
	bool checkFace();
	int index;  //This is the index that the first vertex of this face will appear in the cbo, ibo, and vbo
};
#endif