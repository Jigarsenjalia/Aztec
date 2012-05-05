#ifndef HALFEDGE_H
#define HALFEDGE_H

#include "Vertex.h"
#include "Face.h"
using namespace std;

class Face;
class Vertex;

class HalfEdge {

public:
	Face* f;
	Vertex* v;
	int id;
	int num;
	HalfEdge* next;
	HalfEdge* sym;
	HalfEdge* prev;
	HalfEdge(Vertex*);
	HalfEdge(Vertex*, const int);
	HalfEdge(const int);
	~HalfEdge(void);
	bool subdivided;
	bool drawn;
	Vertex* pV;
	int sharpness;

};
#endif