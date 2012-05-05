#include "Mesh.h"
#include <iostream>
#include <cassert>

/**
 * Constructor.
 *
 * Initializes all variables to 0.
 */
Mesh::Mesh() {
	vcount = 0;
	ecount = 0;
	fcount = 0;
	changed=true;
	mname=0;
}

/**
 * Destructor.
 */
Mesh::~Mesh() {
	while (vertices.size() != 0)
		vertices.pop_back();
	while (edges.size() != 0)
		edges.pop_back();
	while (faces.size() != 0)
		faces.pop_back();
}

/**
 * Indicates whether the mesh's structure has changed.
 *
 * @return
 *  true if the mesh's structure has changed, false otherwise.
 */
bool Mesh::isChanged(){
	return changed;
}

/**
 * Tells the mesh that the client has processed previous changes.
 */
void Mesh::changedFinished(){
	changed=false;
}

/**
 * Adds a vertex to the mesh's list of vertices.
 *
 * @param v - 
 *  the vertex to be added to the mesh
 */
void Mesh::addV(Vertex* v) {
	vertices.push_back(v);
	vcount++;
}

/**
 * Adds a half edge to the mesh's list of half edges.
 *
 * @param he - 
 *  the half edge to be added to the mesh
 */
void Mesh::addHE(HalfEdge* he) {
	if (he){ edges.push_back(he);
	ecount++;
	}
}

/**
 * Adds a face to the mesh's list of faces.
 *
 * @param f - 
 *  the face to be added to the mesh
 */
void Mesh::addF(Face* f) {
	faces.push_back(f);
	fcount++;
}

/**
 * Removes a vertex from the mesh's list of vertices.
 *
 * @param t - 
 *  the num of the vertex to be deleted
 */
void Mesh::deleteV(int t) {
	for(int i=0; i<vertices.size(); i++)
		if (vertices[i]->id == t) {
			vertices.erase(vertices.begin() + i);
			return;
		}
}

/**
 * Removes a half edge from the mesh's list of half edges.
 *
 * @param t - 
 *  the num of the vertex to be deleted
 */
void Mesh::deleteHE(int t) {
	for(int i=0; i<edges.size(); i++)
		if (edges[i]->id == t) {
			edges.erase(edges.begin() + i);
			return;
		}
}

/**
 * Removes a face from the mesh's list of faces.
 *
 * @param t - 
 *  the num of the vertex to be deleted
 */
void Mesh::deleteF(int t) {
	for(int i=0; i<faces.size(); i++)
		if (faces[i]->id == t) {
			faces.erase(faces.begin() + i);
			return;
		}
}

/**
 * Return the number of vertices in the mesh
 *
 * @return
 *  The number of vertices in the mesh
 */
int Mesh::getVSize() {
	return vertices.size();
}

/**
 * Return the number of half edges in the mesh
 *
 * @return
 *  The number of half edges in the mesh
 */
int Mesh::getHESize() {
	return edges.size();
}

/**
 * Return the number of faces in the mesh
 *
 * @return
 *  The number of faces in the mesh
 */
int Mesh::getFSize() {
	return faces.size();
}

/**
 * Gets a vertex from the mesh
 *
 * @param i - 
 *  The integer identifier for a vertex.
 *
 * @return
 *  The vertex with that identifier
 */
Vertex* Mesh::getV(int i) {
	assert(i<vertices.size());
	return vertices[i];
}

/**
 * Gets a half edge from the mesh
 *
 * @param i - 
 *  The integer identifier for a half edge.
 *
 * @return
 *  The half edge with that identifier
 */
HalfEdge* Mesh::getHE(int i) {
	assert(i<edges.size());
	return edges[i];
}

/**
 * Gets a face from the mesh
 *
 * @param i - 
 *  The integer identifier for a face.
 *
 * @return
 *  The face with that identifier
 */
Face* Mesh::getF(int i) {
	assert(i<faces.size());
	return faces[i];
}

Vertex* Mesh::findV(int i) {
	for (int j=0; j<vertices.size(); j++)
		if (vertices[j]->id == i)
			return vertices[j];
	return 0;
}

HalfEdge* Mesh::findHE(int i) {
	for (int j=0; j<edges.size(); j++)
		if (edges[j]->id == i)
			return edges[j];
	return 0;
}

Face* Mesh::findF(int i) {
	for (int j=0; j<faces.size(); j++)
		if (faces[j]->id == i)
			return faces[j];
	return 0;
}

/**
 * Adds a vertex into an edge.
 *
 * @param he0 - 
 *  the half edge whose next will have the next vertex inserted into it
 *
 * @return
 *  the inserted vertex
 */
Vertex* Mesh::addVertex(HalfEdge* he0) {
	HalfEdge* he1 = he0->next;
	if (!he1->sym)
		return 0;
	HalfEdge* he2 = he1->sym;
	HalfEdge* henl = new HalfEdge(ecount);
	henl->sharpness = he1->sharpness;
	HalfEdge* henr = new HalfEdge(ecount+1);
	henr->sharpness = he2->sharpness;
	
	henl->next = he1;
	he0->next = henl;
	henr->next = he2->next;
	he2->next = henr;
	henl->sym = henr;
	henr->sym = henl;
	henl->f = he0->f;
	henr->f = he2->f;
	
	Vertex* vn = new Vertex((he0->v->x+he1->v->x)/2, (he0->v->y+he1->v->y)/2, (he0->v->z+he1->v->z)/2,vcount);
	henl->v = vn;
	henr->v = he2->v;
	he2->v = vn;
	//
	//if (he2==he2->f->he){
	//	he2->f->he=henr;
	//}
	addV(vn);
	addHE(henl);
	addHE(henr);
	order();
	return vn;
}

/**
 * Inserts an edge between two faces.  The faces must share exactly 1 vertex.
 *
 * @param fa, fb - 
 *  the faces between which the new half edge will be inserted
 *
 * @param hea0, heb0 - 
 *  the half edges determine where on the faces the new half edge will be inserted
 *
 * @return
 *  An error message if the method fails. The empty string otherwise.
 */
char* Mesh::addEdge(Face* fa, Face* fb, HalfEdge* hea0, HalfEdge* heb0) {
	if (hea0->f != fa || heb0->f != fb) {
		cout<<"Error in addEdge: Selected edge(s) don't belong to selected face(s)\n";
		return "Selected edge(s) don't belong to selected face(s).";
	}
	else if (hea0->v != heb0->v){
		cout<<"Error in addEdge: Selected edges don't share a vertex\n";
		return "Selected edges don't share a vertex.";
	}
	else if (hea0->next->sym == heb0 || heb0->next->sym == hea0) {
		cout<<"Error in addEdge: Selected faces share more than one vertices\n";
		return "Selected faces share more than one vertices.";
	}
	else {
		Vertex* v0 = hea0->v;
		Vertex* v1 = new Vertex(v0->x, v0->y, v0->z,vcount);
		HalfEdge* henl = new HalfEdge(ecount);
		HalfEdge* henr = new HalfEdge(ecount+1);
		hea0->next->sym->v = v1;
		henl->next = hea0->next;
		hea0->next = henl;
		henl->v = v1;
		henl->f = fa;
		henl->sym = henr;
		henr->next = heb0->next;
		heb0->next = henr;
		henr->v = v0;
		heb0->v = v1;
		henr->f = fb;
		henr->sym = henl;
		addV(v1);
		addHE(henl);
		addHE(henr);
		order();
		return 0;
	}
}

/**
 * Splits a quad into two triangles. Fails if the face is not a quad.
 *
 * @param f1 - 
 *  the face to be split
 *
 * @return
 *  An error message if the method fails. The empty string otherwise.
 */
char* Mesh::splitQuad(Face* f1) {
	if (f1->he->next->next->next->next != f1->he) {
		cout<<"Error in splitQuad\n";
		return "The selected face is not a quad.";
	}
	else {
		HalfEdge* he0 = f1->he;
		Face* f2 = new Face(f1->color,fcount);
		HalfEdge* hel = new HalfEdge(ecount);
		HalfEdge* her = new HalfEdge(ecount+1);
		hel->f = f1;
		her->f = f2;
		f2->he = her;
		hel->sym = her;
		her->sym = hel;
		hel->v = he0->next->next->v;
		hel->next = he0->next->next->next;
		her->next = he0->next;
		her->next->next->next = her;
		he0->next->f = f2;
		he0->next->next->f = f2;
		he0->next = hel;
		her->v = he0->v;
		addF(f2);
		addHE(hel);
		addHE(her);
		order();
		return 0;
	}
}

/**
 * Deletes a vertex from the mesh's structure.
 *
 * @param he0 - 
 *  the half edge that points to the vertex to be deleted
 */
void Mesh::deleteVertex(HalfEdge* he0) {
	Vertex* v0 = he0->v;
	HalfEdge* he1 = he0->next->sym;
	vector<HalfEdge*> hes = vector<HalfEdge*>();
	hes.push_back(he0);
	while (he1!=0 && he1!=he0) {
		hes.push_back(he1);
		he1 = he1->next->sym;
	}
	if (he1 == 0) {
		HalfEdge* he2 = he0->sym;
		while (he2!=0 && he2->v!=v0)
			he2 = he2->next;
		if (he2)
			hes.push_back(he2);
	}
	for (int i=0; i<hes.size(); i++) {
		Face* f = hes[i]->f;
		if (f->verticesCount() > 3) {
			HalfEdge* temp = hes[i]->next;
			hes[i]->v = hes[i]->next->v;
			hes[i]->next = hes[i]->next->next;
			hes[i]->sym = 0;
			if (f->he == temp)
				f->he = hes[i];
			HalfEdge* s = temp->sym;
			if (s)
				s->sym = 0;
			int t = temp->id;
			deleteHE(t);
		}
		else {
			HalfEdge* temp1 = hes[i]->next;
			HalfEdge* temp2 = hes[i]->next->next;
			int t1 = temp1->id;
			int t2 = temp2->id;
			int t3 = hes[i]->id;
			int t4 = f->id;
			HalfEdge* s1 = temp1->sym;
			if (s1)
				s1->sym = 0;
			HalfEdge* s2 = temp2->sym;
			if (s2)
				s2->sym = 0;
			HalfEdge* s3 = hes[i]->sym;
			if (s3)
				s3->sym = 0;
			deleteHE(t1);
			deleteHE(t2);
			deleteHE(t3);
			deleteF(t4);
		}
	}
	int t = v0->id;
	deleteV(t);
	vector<int> toDel = vector<int>();
	for (int i=0; i<vertices.size(); i++) {
		bool nec = false;
		for (int j=0; j<edges.size(); j++)
			if (edges[j]->v == vertices[i]) {
				nec = true;
				break;
			}
		if (!nec)
			toDel.push_back(vertices[i]->id);
	}
	for (int i=0; i<toDel.size(); i++)
		deleteV(toDel[i]);
	order();
}

/**
 * Splits a generic polygon.
 *
 * @param f1 - 
 *  the face to be split
 *
 * @param he1 - 
 *  TODO
 */
void Mesh::splitPolygon(Face* f1, HalfEdge* he1) {
	HalfEdge* he0 = f1->he;
	Face* f2 = new Face(f1->color,fcount);
	HalfEdge* hel = new HalfEdge(ecount);
	HalfEdge* her = new HalfEdge(ecount+1);
	hel->f = f1;
	her->f = f2;
	f2->he = her;
	hel->sym = her;
	her->sym = hel;
	hel->v = he1->v;
	hel->next = he1->next;
	her->next = he0->next;
	he1->next = her;
	HalfEdge* he2 = he0->next;
	while (he2 != he1) {
		he2->f = f2;
		he2 = he2->next;
	}
	he1->f = f2;
	he0->next = hel;
	her->v = he0->v;
	addF(f2);
	addHE(hel);
	addHE(her);
	order();
}

/**
 * Averages a list of vertices into one vertex
 *
 * @param verts - 
 *  the list of vertices to be averaged
 *
 * @return
 *  a vertex whose coordinates are the average of the vertices in the list
 */
Vertex Mesh::average(vector<Vertex> verts){
	float x=0;
	float y=0;
	float z=0;
	float size = verts.size();

	for (int i=0; i<verts.size(); i++){
		x+=verts[i].x;
		y+=verts[i].y;
		z+=verts[i].z;
	}

	x=x/size;
	y=y/size;
	z=z/size;

	return Vertex(x,y,z);
}

/**
 * Smooths a mesh through the Catmull-Clark subdivision algorithm
 */
void Mesh::subdivide(){
	changed = true;
	int numFaces=faces.size();
	glm::vec3 coords;
	for (int i=0; i<numFaces; i++){
		coords = faces[i]->setCentroid();			//Get the x y z of the centroid;
		faces[i]->centroid->id=vcount;
		addV(faces[i]->centroid);
	}

	int opcount = vcount;
	int deletedFaces = 0;
	//Add the edge points
	for (int i=0; i<numFaces; i++){
		if (faces[i]->checkFace()){
			addEdgePoints(faces[i]);
			deletedFaces++;
		}
	}

	moveOrigPoints(opcount);

	bool weirdFaces = false;
	//Add face points
	for (int i=0; i<numFaces; i++){
		if (faces[i]->checkFace())
		addFacePoint(faces[i],faces[i]->he, i);
		else weirdFaces=true;
	}

	if (weirdFaces){
		for (int i=0; i<numFaces; i++){
			if (faces[i]->checkFace()){
				faces.erase(faces.begin() + i);
				i=0;
				numFaces--;
			}
		}
	}
	else{
		for (int i=0; i<numFaces; i++){
			faces.erase(faces.begin());
		}
	}

	for (int i=0; i<faces.size(); i++){
		HalfEdge* current = faces[i]->he;
		int count=0;
		do{
			current->v->isOrig = true;
			current->v->origMoved = false;
			current->subdivided=false;
			current = current->next;
		}while(current != faces[i]->he);
	}
	order();
}

/**
 * Helper method for subdivide that adds a midpoint between each edge of a face
 *
 * @param f - 
 *  the face to add the midpoints to
 */
void Mesh::addEdgePoints(Face* f){
	HalfEdge* current = f->he;	
	HalfEdge* nextHE;
	int sharpness;
	Vertex* v1;
	Vertex* v2;
	Vertex* centroid1;
	Vertex* centroid2;
	Vertex* edgePoint;
	Vertex e;
	do{
		if (current->sharpness > 0){
			current->v->sharpEdges->push_back(current);
		}
		nextHE = current->next;				//Get the half edge that are you going to add an edgePoint to
		if (nextHE->subdivided == false){				//Only subdivide an edge if it has not been subdivided
			sharpness = current->next->sharpness;
			v1 = nextHE->v;						//Get the next half edges vetex
			HalfEdge* symHE = nextHE->sym;				//Get the edge into which were adding the edgepoint
			v2 = symHE->v;						//Get that half edges vertex
			centroid1 = f->centroid;		//Get the two facePoints we are going to average
			if (symHE != 0) centroid2 = symHE->f->centroid;

			edgePoint = addVertex(current);

			//Now, we take the average of v2, v1, and the two centroids
			//and add it to the mesh
			if (edgePoint){
			vector<Vertex> points;						//Add the points that are to be averaged
			points.push_back(*v1);
			if (symHE != 0) points.push_back(*v2);
			if (sharpness <= 0){
				points.push_back(*centroid1);
				points.push_back(*centroid2);
			}
			e = average(points);				//Updates the new edge point's x y z
			edgePoint->x = e.x;
			edgePoint->y = e.y;
			edgePoint->z = e.z;

			current->next->subdivided=true;				//Mark the edges that have been subdivided
			nextHE->subdivided=true;
			current->next->sym->subdivided=true;
			nextHE->sym->subdivided=true;

			points.clear();
			}
			//if (!(f->he->v->isOrig)) f->he = f->he->next;

		}
		current=nextHE;								//Continue loop until you make a full loop
	}while(current!=f->he);
}

/**
 * Helper method for subdivide that adds a centroid to each face
 *
 * @param f - 
 *  the face to add the centroid to
 */
void Mesh::addFacePoint(Face* face, HalfEdge* start, int pos){
	if (!(start->v->isOrig)) start=start->next;
	HalfEdge* current = start;
	Vertex* vn = face->centroid;
	Face* f;
	HalfEdge* nextHE;
	HalfEdge* nextStart;
	HalfEdge* he1;
	HalfEdge* he2;
	do{
		f = new Face(current,face->color,fcount);		//Make a new subdivided face
		addF(f);
		//faces.push_back(f);		//Add this new face to the faces vector

		nextHE = current->next;
		nextStart = nextHE->next;

		he1 = new HalfEdge(ecount);
		he2 = new HalfEdge(ecount+1);
		addHE(he1);
		addHE(he2);

		he1->f=f;				//Update all 4 half edges to point to the new face
		he2->f=f;
		current->f=f;
		nextHE->f=f;

		nextHE->next = he1;		//Update all next pointers
		he1->sharpness = nextHE->sharpness;
		he1->next = he2;
		he2->next = current;
		he2->sharpness = current->sharpness;

		he1->v = vn;			//Update the vertices
		he2->v = current->sym->v;

		he1->sym = nextStart;	//Set temporary syms that tie each new face to each other

		if (current->sharpness > 0){
			current->sharpness--;
		}
		if (nextHE->sharpness > 0){
			nextHE->sharpness--;
		}
		if (he1->sharpness > 0){
			he1->sharpness--;
		}
		if (he2->sharpness > 0){
			he2->sharpness--;
		}

		current=nextStart;		//Update starting edge
	}while(current!=start);

	//Now we have to correctly assign all of the sym pointers
	//We can take advatange of the fact that each new face will have to be a quad
//		HalfEdge* he1;
		HalfEdge* nextCurrent;
		HalfEdge* he1Sym;
		do{
			he1 = current->next->next;
			nextCurrent = he1->sym;						//This is the next starting point for the next face;
			he1Sym = he1->sym->next->next->next;		//This takes advantage of the fact that this face is a quad.
			he1->sym = he1Sym;
			he1Sym->sym = he1;

			current=nextCurrent;
		}while(current!=start);
}

//not used for anything right now....
void Mesh::moveOrigPoints(vector<HalfEdge*> ed){
	//vector<Vertex> facePoints;
	//vector<Vertex> midPoints;
	////HalfEdge* current = edges[i];
	//for (int i=0; i<ed.size(); i++){{
	//	int x = current->v->sharpEdges->size();
	//	current->v->origMoved = true;
	//	//moved++;
	//	int n=0;
	//	if (current->v->sharpEdges->size() <= 1){
	//		do{
	//			n++;
	//			facePoints.push_back(*(current->f->centroid));
	//			midPoints.push_back(*(current->next->v));

	//			current = current->next->sym;
	//		}while(current != edges[i]);

	//		Vertex f = average(facePoints);
	//		Vertex r = average(midPoints);

	//		glm::vec3 F = glm::vec3(f.x, f.y, f.z);
	//		glm::vec3 R = glm::vec3(r.x, r.y, r.z);
	//		glm::vec3 P = glm::vec3(current->v->x, current->v->y, current->v->z);

	//		float nf = (float)n;

	//		glm::vec3 NEWP = (F + 2.0f*R + ((float)n-3.0f)*P)/(float)n;

	//		NEWP = (nf-2)/nf*P + R/(nf) + F/(nf);

	//		current->v->x = NEWP[0];
	//		current->v->y = NEWP[1];
	//		current->v->z = NEWP[2];
	//	}
	//	else if (current->v->sharpEdges->size() == 2){
	//		Vertex* E1 = (current->v->sharpEdges->at(0))->v;
	//		Vertex* E2 = (current->v->sharpEdges->at(1))->v;
	//		
	//		glm::vec3 e1 = glm::vec3(E1->x, E1->y, E1->z);
	//		glm::vec3 e2 = glm::vec3(E2->x, E2->y, E2->z);
	//		glm::vec3 v1 = glm::vec3(current->v->x, current->v->y, current->v->z);
	//		glm::vec3 NEWP = (e1 + e2 + 6.0f*v1)/8.0f;
	//		current->v->x = NEWP[0];
	//		current->v->y = NEWP[1];
	//		current->v->z = NEWP[2];
	//		}
	//	else{
	//		int x=3;
	//	}
	//}
	//current->v->sharpEdges->clear();
	//}
}

/**
 * Helper method for subdivide that moves the original points of a mesh
 *
 * @param numPoints - 
 *  the maximum number of points to move
 */
void Mesh::moveOrigPoints(int numPoints){
	int moved = 0;
	HalfEdge* current;
	for (int i=0; i<edges.size(); i++){
		if (moved==numPoints) 
			return;
		vector<Vertex> facePoints;
		vector<Vertex> midPoints;
		current = edges[i];
		if (current->v->isOrig && current->v->origMoved == false){
			int x = current->v->sharpEdges->size();
			current->v->origMoved = true;
			moved++;
			int n=0;
			if (current->v->sharpEdges->size() <= 1){
				do {
					if (current){
					n++;
					facePoints.push_back(*(current->f->centroid));
					midPoints.push_back(*(current->next->v));
					current = current->next->sym;
					}
				}while(current != edges[i] && current);
	
				if (current){
				Vertex f = average(facePoints);
				Vertex r = average(midPoints);
	
				glm::vec3 F = glm::vec3(f.x, f.y, f.z);
				glm::vec3 R = glm::vec3(r.x, r.y, r.z);
				glm::vec3 P = glm::vec3(current->v->x, current->v->y, current->v->z);
	
				float nf = (float)n;
	
				glm::vec3 NEWP = (F + 2.0f*R + ((float)n-3.0f)*P)/(float)n;
	
				NEWP = (nf-2)/nf*P + R/(nf) + F/(nf);
	
				current->v->x = NEWP[0];
				current->v->y = NEWP[1];
				current->v->z = NEWP[2];
				}
			}
			else if (current->v->sharpEdges->size() == 2){
				Vertex* E1 = (current->v->sharpEdges->at(0))->v;
				Vertex* E2 = (current->v->sharpEdges->at(1))->v;
				
				glm::vec3 e1 = glm::vec3(E1->x, E1->y, E1->z);
				glm::vec3 e2 = glm::vec3(E2->x, E2->y, E2->z);
				glm::vec3 v1 = glm::vec3(current->v->x, current->v->y, current->v->z);

				glm::vec3 NEWP = (e1 + e2 + 6.0f*v1)/8.0f;

				current->v->x = NEWP[0];
				current->v->y = NEWP[1];
				current->v->z = NEWP[2];
			}
			else{
				int x=3;
			}
		}
		if (current) current->v->sharpEdges->clear();
	}
}

//please excuse this method.
//its pretty sloppily done - will go back and fix it!
//define only for quads right now
void Mesh::makeExtrusion(Face* f){
	if (f->he->next->next->next->next != f->he) {
		cout<<"Error in extrude, face was not a quad.\n";
		return;
	}
	changed=true;
	glm::vec3 center = f->setCentroid();
	f->centroid->id = vcount;
	addV(f->centroid);

	HalfEdge* current = f->he;
	vector<Vertex*> newPoints;
	vector<HalfEdge*> newEdges;
	deleteF(f->id);
	//This loop add the points at the midpoint between each vertex of f and f's centroid
	do{

		Vertex* newV = new Vertex((current->v->x + center[0])/2.0f, (current->v->y + center[1])/2.0f, (current->v->z + center[2])/2.0f, vcount);
		newPoints.push_back(newV);
		addV(newV);

		HalfEdge* newHE1 = new HalfEdge(newV, ecount);
		newEdges.push_back(newHE1);
		addHE(newHE1);

		HalfEdge* newHE2 = new HalfEdge(current->v, ecount);
		newHE2->next = current->next;
		addHE(newHE2);

		newHE1->sym = newHE2;
		newHE2->sym = newHE1;
	
		current = current->next;
	}while(current != f->he);
	
	vector<Face*> newFaces;
	int i = 0;
	do{
		HalfEdge* nextHE = current->next;

		current->next = newEdges[i];
		
		HalfEdge* h = new HalfEdge(newEdges[newEdges.size()-1]->v,ecount);
		h->next = newEdges[newEdges.size()-1]->sym;
		if (i!=0){
			h->v = newEdges[i-1]->v;
			h->next = newEdges[i-1]->sym;
		}
		current->next->next = h;
		addHE(h);

		Face* newF = new Face(current, f->color, fcount);
		addF(newF);
		newFaces.push_back(newF);
		current->f = newF;
		newEdges[i]->f = newF;
		h->f = newF;
		h->next->f = newF;

		current = nextHE;
		i+=1;
	}while(current != f->he);

	Face* newFace = new Face(0, f->color, fcount);
	addF(newFace);

	HalfEdge* he1 = new HalfEdge(newFaces[0]->he->next->v, ecount);
	he1->sym = newFaces[0]->he->next->next;
	he1->sym->sym = he1;

	HalfEdge* he2 = new HalfEdge(newFaces[1]->he->next->v, ecount+1);
	he2->sym = newFaces[1]->he->next->next;
	he2->sym->sym = he2;

	HalfEdge* he3 = new HalfEdge(newFaces[2]->he->next->v, ecount+2);
	he3->sym = newFaces[2]->he->next->next;
	he3->sym->sym = he3;

	HalfEdge* he4 = new HalfEdge(newFaces[3]->he->next->v, ecount+3);
	he4->sym = newFaces[3]->he->next->next;
	he4->sym->sym = he4;

	he1->next = he2;
	he2->next = he3;
	he3->next = he4;
	he4->next = he1;

	addHE(he1);
	addHE(he2);
	addHE(he3);
	addHE(he4);

	newFace->he = he1;
	he1->f = newFace;
	he2->f = newFace;
	he3->f = newFace;
	he4->f = newFace;

	order();
}

/**
 * Extrudes a face.
 *
 * @param f - 
 *  the face to extrude
 */
void Mesh::extrude(Face* f){
	changed=true;
	HalfEdge* current = f->he;

	vector<Vertex*> vertices;
	do{
		current->next->pV = current->v;

		Vertex* v = new Vertex(current->v->x, current->v->y, current->v->z, vcount);
		vertices.push_back(v);
		addV(v);

		current = current->next;
	}while(current != f->he);

	int i=0;
	do{
		
		//he0
		HalfEdge* he0 = new HalfEdge(current->pV, ecount);
		addHE(he0);
		he0->sym = current;

		Vertex* prev = current->pV;

		//he1
		/////////////
		HalfEdge* he1 = new HalfEdge(vertices[vertices.size()-1-i], ecount);
		if (i != 0) he1->v = vertices[i-1];
		addHE(he1);

		he0->sym = current;

		//he2
		HalfEdge* he2 = new HalfEdge(vertices[i], ecount);
		addHE(he2);
		he2->sym = current->sym;
		he2->sym->sym = he2;

		//he3
		HalfEdge* he3 = new HalfEdge(current->v, ecount);
		addHE(he3);

		current->sym->v = he1->v;
		current->sym->next->sym->v = he1->v;

		he0->next = he1;
		he1->next = he2;
		he2->next = he3;
		he3->next = he0;

		current->sym = he0;

		Face* newFace = new Face(he0, f->color, fcount);
		addF(newFace);

		he0->f = newFace;
		he1->f = newFace;
		he2->f = newFace;
		he3->f = newFace;

		current = current->next;
		i++;
	}while(current != f->he);

	setPrev();
	//checkStructure();
	
	i=0;
	do{
		HalfEdge* sym1 = current->sym->next;
		HalfEdge* sym2 = current->prev->sym->prev;

		sym1->sym = sym2;
		sym2->sym = sym1;

		HalfEdge* c = current->sym->next;
		Vertex* v = current->sym->next->v;
		do{
			c->v = v;
			c = c->next->sym;
		}while(c != current->sym->next);

		current = current->next;
	}while(current != f->he);
	order();
}

/**
 * Inserts an edge loop. Will only succeed if it does not encounter any triangles
 *
 * @param current - 
 *  the edge to start the edge loop at
 *
 * @return
 *  true if the method can be completed, false otherwise
 */
bool Mesh::insertEdgeLoop(HalfEdge* current){
	if (!checkForEdgeLoop(current)) return false;
	changed = true;
	HalfEdge* original = current;
	do{
		HalfEdge* nextCurrent = current->next->sym->next;		//Get the next now because we are messing with the structure soon

		addVertex(current);

		current = nextCurrent;
	}while(current != original);

	setPrev();

	do{
		deleteF(current->f->id);
		HalfEdge* nextCurrent = current->next->sym->next;		//Get the next now because we are messing with the structure soon

		HalfEdge* newEdge1 = new HalfEdge(ecount);
		addHE(newEdge1);

		newEdge1->v = current->prev->prev->v;
		newEdge1->next = current->prev;

		HalfEdge* newEdge2 = new HalfEdge(ecount);
		addHE(newEdge2);

		newEdge2->next = current->next->next;
		newEdge2->v = current->next->v;

		newEdge1->sym = newEdge2;
		newEdge2->sym = newEdge1;

		current->next->next = newEdge1;

		current->prev->prev->next = newEdge2;

		Face* newFace1 = new Face(current, current->f->color, fcount);
		Face* newFace2 = new Face(newEdge2, current->f->color, fcount+1);

		HalfEdge* c = current;
		do{

			c->f = newFace1;

			c = c->next;
		}while(c != current);

		c = newEdge2;
		do{

			c->f = newFace2;

			c = c->next;
		}while(c != newEdge2);


		addF(newFace1);
		addF(newFace2);
		

		current = nextCurrent;
	}while(current != original);

	order();
	return true;
}

/**
 * Helper for edgeLoop that checks that every face that would have a loop inserted into them is a quad
 *
 * @param current - 
 *  the edge at which to start checking
 *
 * @return
 *  true if the face is a quad, false otherwise
 */
bool Mesh::checkForEdgeLoop(HalfEdge* current){
	current = current->next;
	HalfEdge* original = current;

	do{
		if (current->next->next->next->next != current){
			return false;
		}
		cout<<current->id<<endl;
		current = current->next->next->sym;
	}while(current!=original);

	return true;
}

void Mesh::poke(Face* f){
	
}

/**
 * Translates a face
 *
 * @param f - 
 *  the face to be translated
 *
 * @param dir - 
 *  the direction to translate
 */
void Mesh::translate(Face* f, glm::vec3 dir){
	HalfEdge* current = f->he;

	do{
			
		current->v->x += dir[0];
		current->v->y += dir[1];
		current->v->z += dir[2];

		current = current->next;
	}while(current != f->he);
}

/**
 * Scales a face
 *
 * @param f - 
 *  the face to be scaled
 *
 * @param factor - 
 *  the amount to scale
 */void Mesh::scale(Face *f, float factor){
	HalfEdge* current = f->he;

	do{

		current->v->x = (current->v->x)*factor;
		current->v->y = (current->v->y)*factor;
		current->v->z = (current->v->z)*factor;

		current = current->next;
	}while(current != f->he);
}

/**
 * Orders the mesh's vertices, faces, and half edges
 */
void Mesh::order() {
	for (int i=0; i<faces.size(); i++)
		faces[i]->num = i;
	for (int i=0; i<edges.size(); i++)
		edges[i]->num = i;
	for (int i=0; i<vertices.size(); i++)
		vertices[i]->num = i;
}

/**
 * Set the previous pointer of the mesh
 */
void Mesh::setPrev(){
	for (int i=0; i<faces.size(); i++){
		Face* f = faces[i];

		HalfEdge* current = f->he;

		do{

			current->next->prev = current;

			current = current->next;
		}while(current != f->he);
	}
}

/**
 * Checks the structure of the mesh, printing any errors
 */
void Mesh::checkStructure(){
		for (int i=0; i<faces.size(); i++){
		Face* ff = faces[i];

		HalfEdge* c = ff->he;

		do{

			if (c->sym->sym != c){
				cout<<"a sym pointer is wrong"<<endl;
			}

			if (c->next->prev != c){
				cout<<"a prev pointer is wrong"<<endl;
			}

			c = c->next;
		}while(c!=ff->he);
	}
}