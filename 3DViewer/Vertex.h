#ifndef VERTEX_H
#define VERTEX_H

#include "../glm/glm.hpp"
#include "HalfEdge.h"
#include <vector>

using namespace std;

class HalfEdge;

class Vertex {

public:
	float x;
	float y;
	float z;
	vector<HalfEdge*>* sharpEdges;
	int id;
	int num;
	bool origMoved;
	bool isOrig;
	Vertex(const float, const float, const float);
	Vertex(const float, const float, const float, const int);
	Vertex(void);
	glm::vec3 toVec();
	HalfEdge* he;
};
#endif