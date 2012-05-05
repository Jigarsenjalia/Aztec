#include "Vertex.h"

Vertex::Vertex(const float x1, const float y1, const float z1) {
	x = x1;
	y = y1;
	z = z1;
	isOrig=false;
	origMoved=false;
	sharpEdges = new vector<HalfEdge*>();
}

Vertex::Vertex(const float x1, const float y1, const float z1, const int i1) {
	x = x1;
	y = y1;
	z = z1;
	id = i1;
	isOrig=false;
	origMoved=false;
	sharpEdges = new vector<HalfEdge*>();
}

Vertex::Vertex() {
	x = 0;
	y = 0;
	z = 0;
	isOrig=false;
	origMoved=false;
	sharpEdges = new vector<HalfEdge*>();
}

glm::vec3 Vertex::toVec() {
	return glm::vec3(x,y,z);
}