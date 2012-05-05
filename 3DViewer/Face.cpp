#include "Face.h"
#include <cfloat>
#include <vector>

Face::Face(HalfEdge* he1, const float c1[3]) {
	he = he1;
	color[0] = c1[0];
	color[1] = c1[1];
	color[2] = c1[2];
	centroid = 0;
}

Face::Face(HalfEdge* he1, const float c1[3], const int i1) {
	he = he1;
	color[0] = c1[0];
	color[1] = c1[1];
	color[2] = c1[2];
	id = i1;
	centroid = 0;
}

Face::Face(const float c1[3], const int i1) {
	color[0] = c1[0];
	color[1] = c1[1];
	color[2] = c1[2];
	id = i1;
	centroid = 0;
}

Face::~Face() {
}

int Face::verticesCount() {
	int count = 1;
	Vertex* v0 = he->v;
	HalfEdge* h = he->next;
	while (h->v != v0) {
		count ++;
		h = h->next;
	}
	return count;
}

glm::vec3 Face::normal() {
	glm::vec3 v1 = he->v->toVec() - he->next->v->toVec();
	glm::vec3 v2 = he->next->v->toVec() - he->next->next->v->toVec();
	glm::vec3 n = glm::cross(v1, v2);
	HalfEdge* he1 = he;
	HalfEdge* he2 = he->next; 
	he1 = he1->next;
	while (glm::length(v1) <= FLT_EPSILON && he1 != he) {
		v1 = he1->v->toVec() - he1->next->v->toVec();
		n = glm::cross(v1, v2);
		he1 = he1->next;
	}
	he2 = he2->next;
	while (glm::length(n) <= FLT_EPSILON && he2 != he->next) {
		v2 = he2->v->toVec() - he2->next->v->toVec();
		n = glm::cross(v1, v2);
		he2 = he2->next;
	}
	n = glm::normalize(n);
	return n;
}

bool Face::checkFlat() {
	if (he->next->next->next->next != he)
		return true; // face is not quad, no need to test
	else {
		glm::vec3 v1 = he->v->toVec() - he->next->v->toVec();
		glm::vec3 v2 = he->next->v->toVec() - he->next->next->v->toVec();
		glm::vec3 v3 = he->next->next->v->toVec() - he->next->next->next->v->toVec();
		glm::vec3 v4 = he->next->next->next->v->toVec() - he->v->toVec();
		glm::vec3 n1 = glm::cross(v1,v2);
		glm::vec3 n2 = glm::cross(v3,v4);
		if (glm::length(n1) > FLT_EPSILON && glm::length(n2) > FLT_EPSILON) {
			n1 = glm::normalize(n1);
			n2 = glm::normalize(n2);
			if (glm::length(n1-n2) > FLT_EPSILON)
				return false;
		}
		n1 = glm::cross(v4,v1);
		n2 = glm::cross(v2,v3);
		if (glm::length(n1) > FLT_EPSILON && glm::length(n2) > FLT_EPSILON) {
			n1 = glm::normalize(n1);
			n2 = glm::normalize(n2);
			if (glm::length(n1-n2) > FLT_EPSILON) {
				he = he->next;
				return false;
			}
		}
		return true;
	}
}

HalfEdge* Face::checkPlanarity() {
	if (verticesCount() < 4)
		return 0;
	vector<Vertex*> vertices = vector<Vertex*>();
	HalfEdge* e = he;
	for (int j=0; j<verticesCount(); j++) {
			vertices.push_back(e->v);
			e = e->next;
		}
	vector<glm::vec3> normals = vector<glm::vec3>();
	for (int i=2; i<verticesCount(); i++) {
		glm::vec3 n = glm::cross((vertices[i]->toVec() - vertices[i-1]->toVec()), (vertices[0]->toVec() - vertices[i]->toVec()));
		n = glm::normalize(n);
		normals.push_back(n);
	}
	int k = 0;
	while (k<normals.size()-1) {
		if (glm::length(normals[k]) <= FLT_EPSILON)
			k++;
		else {
			int l = k+1;
			while(l<normals.size() && glm::length(normals[l]) <= FLT_EPSILON)
				l++;
			if (glm::length(normals[k]) > FLT_EPSILON && glm::length(normals[l]) > FLT_EPSILON && glm::length(normals[k]-normals[l]) > FLT_EPSILON) {
				HalfEdge* e = he;
				for (int i=0; i<k+2; i++)
					e = e->next;
				return e;
			}
			k = l;
		}
	}
	return 0;
}

HalfEdge* Face::split() {
	if (verticesCount() < 4)
		return 0;
	else
		return he->next->next;
}

glm::vec3 Face::setCentroid(){
	if (centroid ==0){
		centroid=new Vertex();
	}
	float x=0;							//The x position of the centroid
	float y=0;							//The y position of the centroid
	float z=0;							//The z position of the centroid

	float numPoints=0;					//Number of points on the face

	HalfEdge* current=he;				//Start at the beginning half edge of the face

	do{

		numPoints+=1;					//Increment numbers of points
		x+=current->v->x;				//Increment x
		y+=current->v->y;				//Increment y
		z+=current->v->z;				//Increment z
		current = current->next;		//Move forward along the face

	}while (current!=he);				//Continue until you make a complete loop

	x = x/numPoints;					//Average x
	y = y/numPoints;					//Average y
	z = z/numPoints;					//Average z

	centroid->x=x;
	centroid->y=y;
	centroid->z=z;

	return glm::vec3(x,y,z);
}

bool Face::checkFace(){
	HalfEdge* edge = he;
	HalfEdge* current = he;
	do{

		if (current->sym == 0){
			return false;
		}
		current = current->next;
	}while(current != he);
	return true;
}