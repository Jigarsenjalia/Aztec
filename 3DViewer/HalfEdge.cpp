#include "HalfEdge.h"

HalfEdge::HalfEdge(Vertex* v1) {
	v = v1;
	subdivided=false;
	sharpness=0;
	drawn = false;
}

HalfEdge::HalfEdge(Vertex* v1, const int i1) {
	v = v1;
	id = i1;
	sym = 0;
	subdivided=false;
	sharpness=0;
	drawn = false;
}

HalfEdge::HalfEdge(const int i1) {
	id = i1;
	subdivided=false;
	sharpness=0;
	drawn = false;
}

HalfEdge::~HalfEdge() {
}