#ifndef MESH_H
#define MESH_H

#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"
#include <vector>
using namespace std;

class Mesh {

private:
	vector<Vertex*> vertices;
	vector<HalfEdge*> edges;
	vector<Face*> faces;

public:
	Mesh(void);
	~Mesh(void);
	int vcount;
	int ecount;
	int fcount;
	void addV(Vertex*);
	void addHE(HalfEdge*);
	void addF(Face*);
	void deleteV(int);
	void deleteHE(int);
	void deleteF(int);
	int getVSize(void);
	int getHESize(void);
	int getFSize(void);
	Vertex* getV(int);
	HalfEdge* getHE(int);
	Face* getF(int);
	Vertex* findV(int); // find vertex, halfedge, face by id
	HalfEdge* findHE(int);
	Face* findF(int);
	Vertex* addVertex(HalfEdge*); // add a vertex into an edge
	char* addEdge(Face*, Face*, HalfEdge*, HalfEdge*); // add an edge between two faces that share a vertex
	char* splitQuad(Face*);
	void deleteVertex(HalfEdge*);
	void splitPolygon(Face*, HalfEdge*);
	bool checkForEdgeLoop(HalfEdge* current);

	void order();
	
	bool changed;
	bool isChanged();
	void changedFinished();

	void setPrev();

	void checkStructure();

	//Average n points
	Vertex average(vector<Vertex>);

	/////////////////////////////////
	/////////////////////////////////
	//Edit Mesh Methods
	/////////////////////////////////
	/////////////////////////////////

	//Catmull-Clark Subdivision//////
	void subdivide();
	//Add the edge points
	void addEdgePoints(Face*);
	//Add the face point from Catmull-Clark
	void addFacePoint(Face*, HalfEdge*, int);
	//Move original points
	void moveOrigPoints(int numPoints);
	void moveOrigPoints(vector<HalfEdge*>);
	/////////////////////////////////

	//Extrusions/////////////////////
	void makeExtrusion(Face*);
	void extrude(Face*);
	/////////////////////////////////

	bool insertEdgeLoop(HalfEdge*);
	void poke(Face*);
	////////////////////////////////
	////////////////////////////////
	////////////////////////////////

	void translate(Face*, glm::vec3);
	void scale(Face*, float);

	char* mname;
};
#endif