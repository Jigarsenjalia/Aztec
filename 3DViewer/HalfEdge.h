#ifndef HALFEDGE_H
#define HALFEDGE_H

#include "Vertex.h"
#include "Face.h"
using namespace std;

class Face;
class Vertex;

class HalfEdge {

public:
    //Constructors////////////////////////////
    HalfEdge(Vertex*);
	HalfEdge(Vertex*, const int);
	HalfEdge(const int);
    //////////////////////////////////////////
    
    //Destructor
	~HalfEdge(void);
    
    //Instance fields
	Face* f;    //The face that contains this half edge
	Vertex* v;  //The vertex that this half edge points to
	int id;
	int num;    //Unique identifier
	HalfEdge* next;
	HalfEdge* sym;
	HalfEdge* prev;
	bool subdivided;    //For subdivision - lets the algorithm know that this edge has been subdivided
	bool drawn;
	Vertex* pV;         //Previous vertex
	int sharpness;      //Sharpness for subdivision
    //////////////////////////////////////////

};
#endif