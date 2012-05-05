#include "myglwidget.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include "../glm/gtc/matrix_transform.hpp"
#include <cstring>
using namespace std;


//TODO when mesh->changed:
//updateScene();
//updatePoints():  //the lattice deformer


#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
#define DEG2RAD (M_PI/180.0)
#define RAD2DEG (180.0/M_PI)

//MyGLWidget::MyGLWidget(QWidget* parent) : QGLWidget(parent) {
//	//This starter code uses a timer to make a square rotate without
//	//user intervention.  You can remove this.
//	setFocusPolicy(Qt::StrongFocus);
//	timer = new QTimer(this);
//	connect(timer, SIGNAL(timeout()), this, SLOT(rotate()));
//	rotation = 0;
//	mouseButton = 2;
//	setMouseTracking(true);
//
//	//We can set our vertex attributes rather than requesting their locations
//	//I chose to do so in this constructor, maybe you would in your init() function?
//	positionLocation = 0;
//	normalLocation = 1;
//	colorLocation = 2;
//	ray = false;
//	mousePressed = false;
//	keyPressed = -1; // not alt
//	face = -1;
//	oldFace = -1;
//	face1 = -1;
//	oldFace1 = -1;
//	attributes = 0;
//	meshLoaded = false;
//	currentMode = FACE;
//	//FFD
//	l = 2;
//	m = 2;
//	n = 2;
//	xn = -1;
//	yn = -1;
//	zn = -1;
//	ffd = false;
//	forcePlanarity = false;
//	//global deformation
//	twrate = 0;
//	tarate = 0;
//	deformx = false;
//	deformy = false;
//	deformz = false;
//	bmin = 0;
//	bcen = 0;
//	bmax = 0;
//	curv = 0;
//
//	oldPoint = glm::vec3(0,0,0);
//	drawFrame = true;
//	edgeLoopEdge = -1;
//	selectedEdge = -1;
//	currentDisplay = SOLID;
//	currentAction = MOVE;
//
//	numPoints=0;
//	numInds=0;
//	vertexArray=0;
//	colorArray=0;
//	normalArray=0;
//	indexArray=0;
//	mesh = new Mesh();
//	unitCube();
//	emit sendMeshInfo(mesh);
//
//	//material and shader
//	mtlName="";
//	bKd=bKa=bKs=bNs=false;
//	lightX=lightY=lightZ=5;
//	Ka=glm::vec4(0.2,0.2,0.2,1.0f);
//	Kd=glm::vec4(0.8,0.8,0.8,1.0f);
//	Ns=0.0f;
//	Ks=glm::vec4(1.0f,1.0f,1.0f,1.0f);
//	
//	//camera
//	lastRot=glm::mat3(1.0f);
//	thisRot=glm::mat3(1.0f);
//	mouseClicked=mouseDragged=false;
//	allowRotation=true;
//	panAx=XY;
//	oldMouseX=0;
//	oldMouseY=0;
//	cameraType=DUMMY;
//}

MyGLWidget::MyGLWidget(QWidget* parent) : QGLWidget(parent) {
	//This starter code uses a timer to make a square rotate without
	//user intervention.  You can remove this.
	setFocusPolicy(Qt::StrongFocus);
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(rotate()));
	rotation = 0;
	mouseButton = 2;
	setMouseTracking(true);

	//We can set our vertex attributes rather than requesting their locations
	//I chose to do so in this constructor, maybe you would in your init() function?
	positionLocation = 0;
	normalLocation = 1;
	colorLocation = 2;
	ray = false;
	mousePressed = false;
	keyPressed = -1; // not alt
	face = -1;
	oldFace = -1;
	face1 = -1;
	oldFace1 = -1;
	attributes = 0;
	meshLoaded = false;
	fast = false;
	currentMode = FACE;
	//FFD
	l = 2;
	m = 2;
	n = 2;
	xn = -1;
	yn = -1;
	zn = -1;
	ffd = false;
	gd = false;

	//global deformation
	twrate = 0;
	tarate = 0;
	deformx = false;
	deformy = false;
	deformz = false;
	bmin = 0;
	bcen = 0;
	bmax = 0;
	curv = 0;

	oldPoint = glm::vec3(0,0,0);
	drawFrame = true;
	edgeLoopEdge = -1;
	selectedEdge = -1;
	currentDisplay = SOLID;
	currentAction = MOVE;

	numPoints=0;
	numInds=0;
	vertexArray=0;
	colorArray=0;
	normalArray=0;
	indexArray=0;
	mesh = new Mesh();
	unitCube();
	emit sendMeshInfo(mesh);

	//material and shader
	mtlName="";
	bKd=bKa=bKs=bNs=false;
	lightX=lightY=lightZ=5;
	Ka=glm::vec4(0.2,0.2,0.2,1.0f);
	Kd=glm::vec4(0.8,0.8,0.8,1.0f);
	Ns=0.0f;
	Ks=glm::vec4(1.0f,1.0f,1.0f,1.0f);
	Tf=glm::vec4(1.0f,1.0f,1.0f,1.0f);
	
	//camera
	lastRot=glm::mat3(1.0f);
	thisRot=glm::mat3(1.0f);
	mouseClicked=mouseDragged=false;
	allowRotation=true;
	panAx=XY;
	oldMouseX=0;
	oldMouseY=0;
	cameraType=DUMMY;

	bar = 0;
}

MyGLWidget::~MyGLWidget() {
	//Release your resources, just like you'd delete pointers
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cbo);
	glDeleteBuffers(1, &nbo);
	glDeleteBuffers(1, &ibo);

	//TODO
	//DELETE VERTEX VBO

	//Tear down the shader program in reverse of building it
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);

	delete timer;
	timer = 0;
	delete arcballCamera;
	arcballCamera=0;
}

void MyGLWidget::initializeGL() {
		emit initSignal(this);
	//Initialize GLEW to get access to latest OpenGL support on your computer
	//GLEW 1.7.0 supplied with this starter code, it's always a good idea to update
	//about once every year or so as OpenGL is updated
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cerr << "GLEW is not initialized!";
	}

	//Create the VBOs and IBO we'll be using to render images in OpenGL
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &ibo);
	
	//VERTEX CBO
	glGenBuffers(1,&vertex_cbo);
	glGenBuffers(1,&vertex_nbo);
	glGenBuffers(1,&vertex_ibo);
	glGenBuffers(1,&vertex_vbo);
	
	//Everybody does this
	glClearColor(0.6, 0.6, 0.6, 1);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);

	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//here is stuff for setting up our shaders
	fragFile = "diffuseFrag2.frag";
	vertFile = "diffuseVert2.vert";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
	
	//load up the source, compile and link the shader program
	const char* vertSource = textFileRead(vertFile);
	const char* fragSource = textFileRead(fragFile);
	glShaderSource(vertexShader, 1, &vertSource, 0);
	glShaderSource(fragmentShader, 1, &fragSource, 0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//For your convenience, i decided to throw in some compiler/linker output helper functions
	//from CIS 565
	GLint compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(vertexShader);
	} 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(fragmentShader);
	} 

	//set the attribute locations for our shaders
	glBindAttribLocation(shaderProgram, positionLocation, "vs_position");
	glBindAttribLocation(shaderProgram, normalLocation, "vs_normal");
	glBindAttribLocation(shaderProgram, colorLocation, "vs_color");

	//finish shader setup
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check for linking success
	GLint linked;
	glGetProgramiv(shaderProgram,GL_LINK_STATUS, &linked);
	if (!linked) 
	{
		printLinkInfoLog(shaderProgram);
	}

	//Get the uniform locations for our shaders, unfortunately they can not be set by us, we have
	u_modelMatrixLocation = glGetUniformLocation(shaderProgram, "u_modelMatrix");
	u_projMatrixLocation = glGetUniformLocation(shaderProgram, "u_projMatrix");
	u_lightPosition=glGetUniformLocation(shaderProgram, "u_lightPosition");
	u_ambient=glGetUniformLocation(shaderProgram, "u_ambient");
	u_camera_position=glGetUniformLocation(shaderProgram, "u_camera_position");
	u_spec=glGetUniformLocation(shaderProgram, "u_specular");
	u_cosine=glGetUniformLocation(shaderProgram, "u_cosine_power");

	//Always remember that it doesn't do much good if you don't have OpenGL actually use the shaders
	glUseProgram(shaderProgram);

	numPoints=0;
	numInds=0;
	vertexArray=0;
	colorArray=0;
	normalArray=0;
	indexArray=0;
	mesh = new Mesh();
	unitCube();
	emit sendMeshInfo(mesh);
	
	// Set camera
	glm::vec3 ref = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 cam = glm::vec3(0.0f,0.0f,10.0f);
	glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
	float fovy = 45;
	float aspect=this->width()/this->height();
	float zfront =  0.1;
	float zback = 100;

	arcballCamera=new arcballCam(ref, cam, up, fovy, this->width(), this->height());

	quaternion=glm::vec4(0,0,0,1);
	lastCam=rayPos=cam;
}

void MyGLWidget::paintGL() {
	//Always and only do this at the start of a frame, it wipes the slate clean
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//create an identity matrix for the model matrix
	glm::mat4 model = glm::mat4(1.0);
	if (currentDisplay != WIREFRAME){ 
		createCube(model);

	}
	if (drawFrame && !fast){
		drawWireframe(model);
	}
	drawGrid(model);
	if (forcePlanarity) updateScene();
}

void MyGLWidget::createCube(glm::mat4 model) {
	if (!fast) {
		ind = 1;
		//if (mesh->changed) {
			//mesh->changed = false;
			int size = mesh->getFSize();
			for (int cur=0; cur<size; cur++){
				Face* f = mesh->getF(cur);
				vector<Vertex*> vertices1 = vector<Vertex*>();
				vector<int> indices1 = vector<int>();

				HalfEdge* he = f->he;
			
				for (int j=0; j<f->verticesCount(); j++) {
					vertices1.push_back(he->v);
					(he->v)->he=he;
					he = he->next;
				}

				numPoints = vertices1.size();
				float* vertices = new float[numPoints * 4];
				for (int j=0; j<numPoints; j++) {
					glm::vec3 v = vertices1[j]->toVec();
					vertices[j*4] = v[0];
					vertices[j*4+1] = v[1];
					vertices[j*4+2] = v[2];
					vertices[j*4+3] = 1.0f;
				}
				vertexArray=vertices;

				float* normals = new float[numPoints * 4];
				float* colors = new float[numPoints * 4];
		
				int c = 0;
				int s = 0;
				ind = 1;

				f->index = s*4;
				for (int j=0; j<f->verticesCount(); j++) {
					glm::vec3 normal = f->normal();
					normals[s*4] = normal[0];
					normals[s*4+1] = normal[1];
					normals[s*4+2] = normal[2];
					normals[s*4+3] = 0.0f;
					s++;
					if (j<f->verticesCount()-2){
						indices1.push_back(ind);	
						indices1.push_back(ind+j+1);
						if (j == f->verticesCount()-3)
							indices1.push_back(ind-1);
						else
							indices1.push_back(ind+j+2);
					}

					colors[c*4] = f->color[0];
					colors[c*4+1] = f->color[1];
					colors[c*4+2] = f->color[2];
					colors[c*4+3] = Kd[3];

					if (face1 == f->num){
						colors[c*4] = f->color[0] + 0.3;
						colors[c*4+1] = f->color[1] + 0.3;
						colors[c*4+2] = f->color[2] + 0.3;
						colors[c*4+3] = Kd[3];
					}

					bool isSelected = false;
					for (int col = 0; col<selectedFaces.size(); col++){
						if (selectedFaces[col] == f->num){
							isSelected = true;
							break;
						}
					}
					if (isSelected){
						colors[c*4] = f->color[0]+0.7;
						colors[c*4+1] = f->color[1]+0.7;
						colors[c*4+2] = f->color[2]+0.7;
						colors[c*4+3] = Kd[3];
					}

					c++; //haha get it

				}

				colorArray=colors;
				normalArray=normals;

				numInds=indices1.size();
				unsigned short* indices = new unsigned short[indices1.size()];
				for (int j=0; j<indices1.size(); j++){
					indices[j] = (unsigned short)indices1[j];
				}
				indexArray=indices;
		//}


			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), vertices, GL_STATIC_DRAW);
	
			glBindBuffer(GL_ARRAY_BUFFER, nbo);
			glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), normals, GL_STATIC_DRAW); 
	
			glBindBuffer(GL_ARRAY_BUFFER, cbo);
			//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
			glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), colors, GL_STATIC_DRAW);	
	
			//activate our three kinds of information
			glEnableVertexAttribArray(positionLocation);
			glEnableVertexAttribArray(colorLocation);
			glEnableVertexAttribArray(normalLocation);
	
			//we're using the vertex data first
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
			glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
		
			//now use color data
			glBindBuffer(GL_ARRAY_BUFFER, cbo);
			glVertexAttribPointer(colorLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
			//one more time with the normals
			glBindBuffer(GL_ARRAY_BUFFER, nbo);
			glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
			//the last thing we need to do is setup our indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInds * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	
			//light and materials!
			glUniform3f(u_lightPosition, lightX, lightY, lightZ);

			glUniform4f(u_ambient, Ka[0], Ka[1], Ka[2],Ka[3]);		//ambient
			glUniform4f(u_spec, Ks[0], Ks[1], Ks[2],Ks[3]);			//specular
			glUniform1f(u_cosine, Ns);								//cosine power

			glm::vec3 u_cam=arcballCamera->getDummyCam();
			glUniform3f(u_camera_position, u_cam[0], u_cam[1], u_cam[2]);
	
			//draw the elements
			glDrawElements(GL_TRIANGLES, numInds, GL_UNSIGNED_SHORT, 0);	
		
			//shut off the information since we're done drawing
			glDisableVertexAttribArray(positionLocation);
			glDisableVertexAttribArray(colorLocation);
			glDisableVertexAttribArray(normalLocation);
	
			if (currentMode==VERTEX)
				drawVertex(model, numPoints, numInds, vertexArray, indexArray, vertices1, false);

			vertices1.clear();
			indices1.clear();
			delete [] indices;
			delete [] normals;
			delete [] vertices;
			delete [] colors;
		}
	}

	else {
		vector<Vertex*> vertices1 = vector<Vertex*>();
		vector<int> indices1 = vector<int>();

		if (mesh->isChanged()){
			mesh->changedFinished();
		//	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotation,glm::vec3(0,1,0));
		//	model = rot * model;

			// vertex coords array, remember the homogeneous representation?
			int size = mesh->getFSize();

			for (int i=0; i<size; i++) {
				Face* f = mesh->getF(i);
				HalfEdge* he = f->he;
				for (int j=0; j<f->verticesCount(); j++) {
					vertices1.push_back(he->v);
					he = he->next;
				}
			}

			numPoints = vertices1.size();
			float* vertices = new float[numPoints * 4];
			for (int i=0; i<numPoints; i++) {
				glm::vec3 v = vertices1[i]->toVec();
				vertices[i*4] = v[0];
				vertices[i*4+1] = v[1];
				vertices[i*4+2] = v[2];
				vertices[i*4+3] = 1.0f;
			}
			vertexArray=vertices;

	
			float* normals = new float[numPoints * 4];
			float* colors = new float[numPoints * 4];
		
			int c = 0;
			int n = 0;
			int ind = 1;
			for (int i=0; i<size; i++) {
				Face* f = mesh->getF(i);
				f->index = n*4;
				for (int j=0; j<f->verticesCount(); j++) {
					normals[n*4] = f->normal()[0];
					normals[n*4+1] = f->normal()[1];
					normals[n*4+2] = f->normal()[2];
					normals[n*4+3] = 0.0f;
					n++;

					if (j<f->verticesCount()-2){
						indices1.push_back(ind);	
						indices1.push_back(ind+j+1);
						if (j == f->verticesCount()-3)
							indices1.push_back(ind-1);
						else
							indices1.push_back(ind+j+2);
					}

					colors[c*4] = f->color[0];
					colors[c*4+1] = f->color[1];
					colors[c*4+2] = f->color[2];
					colors[c*4+3] = 1.0f;
					c++; //haha get it

				}
				ind += f->verticesCount();
			}
		
			colorArray=colors;
			normalArray=normals;

			numInds=indices1.size();
			unsigned short* indices = new unsigned short[indices1.size()];
			for (int i=0; i<indices1.size(); i++){
				indices[i] = (unsigned short)indices1[i];
			}
			indexArray=indices;
		}

		//////////////////////////////////////////////////
		//////////////////////////////////////////////////
		if (oldFace != -1){
			Face* f = mesh->getF(oldFace);
			HalfEdge* current = f->he;
			int i = f->index;
			do{
				colorArray[i] = f->color[0];
				colorArray[i+1] = f->color[1];
				colorArray[i+2] = f->color[2];
	
				i+=4;
			current = current->next;
			}while(current != f->he);
		}

		if (oldFace1 != -1){
			Face* f = mesh->getF(oldFace1);
			HalfEdge* current = f->he;
			int i = f->index;
			do{
				colorArray[i] = f->color[0];
				colorArray[i+1] = f->color[1];
				colorArray[i+2] = f->color[2];

				i+=4;
				current = current->next;
			}while(current != f->he);
		}

		if (face1 != -1){

			oldFace1 = face1;

			Face* f = mesh->getF(face1);
			HalfEdge* current = f->he;
			int i = f->index;
			do{
				colorArray[i] = f->color[0] + 0.3;
				colorArray[i+1] = f->color[1] + 0.3;
				colorArray[i+2] = f->color[2] + 0.3;

				i+=4;
				current = current->next;
			}while(current != f->he);
		}

		if (face != -1){
			oldFace = face;

			Face* f = mesh->getF(face);
			HalfEdge* current = f->he;
			int i = f->index;
			do{
				colorArray[i] = f->color[0] + 0.7;
				colorArray[i+1] = f->color[1] + 0.7;
				colorArray[i+2] = f->color[2] + 0.7;

				i+=4;
				current = current->next;
			}while(current != f->he);
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), vertexArray, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), normalArray, GL_STATIC_DRAW); 

		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
		glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), colorArray, GL_STATIC_DRAW);	

		//activate our three kinds of information
		glEnableVertexAttribArray(positionLocation);
		glEnableVertexAttribArray(colorLocation);
		glEnableVertexAttribArray(normalLocation);

		//we're using the vertex data first
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
		glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
		//now use color data
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glVertexAttribPointer(colorLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

		//one more time with the normals
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

		//the last thing we need to do is setup our indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInds * sizeof(unsigned short), indexArray, GL_STATIC_DRAW);

		//set the model uniform
		//GL_FALSE is used for not doing transpose operation, because glm has already provided us column major matrices.
		//if the matrices you used is row major, change this parameter to GL_TRUE.
		glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
		glUniform3f(u_lightPosition, lightX, lightY, lightZ);
		glUniform4f(u_ambient, Ka[0], Ka[1], Ka[2],Ka[3]);		//ambient
		glUniform4f(u_spec, Ks[0], Ks[1], Ks[2],Ks[3]);			//specular
		glUniform1f(u_cosine, Ns);								//cosine power

		glm::vec3 u_cam=arcballCamera->getDummyCam();
		glUniform3f(u_camera_position, u_cam[0], u_cam[1], u_cam[2]);

		//draw the elements
		glDrawElements(GL_TRIANGLES, numInds, GL_UNSIGNED_SHORT, 0);	
	
		//shut off the information since we're done drawing
		glDisableVertexAttribArray(positionLocation);
		glDisableVertexAttribArray(colorLocation);
		glDisableVertexAttribArray(normalLocation);

		if (currentMode==VERTEX)
			drawVertex1(model, numPoints, numInds, vertexArray, indexArray);

		vertices1.clear();
		indices1.clear();
		
	}
	
	if (ffd) {
		float* ctrlpts = new float[l*m*n*4];
		for (int i=0; i<l; i++)
			for (int j=0; j<m; j++)
				for (int k=0; k<n; k++) {
					ctrlpts[(i*m*n + j*n + k) * 4] = p[i][j][k][0];
					ctrlpts[(i*m*n + j*n + k) * 4 + 1] = p[i][j][k][1];
					ctrlpts[(i*m*n + j*n + k) * 4 + 2] = p[i][j][k][2];
					ctrlpts[(i*m*n + j*n + k) * 4 + 3] = 1.0f;
				}
		unsigned short* ctrlins = new unsigned short[l*m*n];
		for (int i=0; i<l*m*n; i++)
			ctrlins[i] = i;
		vector<Vertex*> g;
		drawVertex(model, l*m*n*4, l*m*n, ctrlpts, ctrlins,g,true);
		delete [] ctrlpts;
		delete [] ctrlins;

		glBegin(GL_LINES);
		for(int i=0; i<l; i++)
			for (int j=0; j<m; j++)
				for (int k=0; k<n-1; k++) {
					glVertex3f(p[i][j][k][0], p[i][j][k][1], p[i][j][k][2]);
					glVertex3f(p[i][j][k+1][0], p[i][j][k+1][1], p[i][j][k+1][2]);
				}
		for(int i=0; i<l; i++)
			for (int k=0; k<n; k++)
				for (int j=0; j<m-1; j++) {
					glVertex3f(p[i][j][k][0], p[i][j][k][1], p[i][j][k][2]);
					glVertex3f(p[i][j+1][k][0], p[i][j+1][k][1], p[i][j+1][k][2]);
				}
		for(int k=0; k<n; k++)
			for (int j=0; j<m; j++)
				for (int i=0; i<l-1; i++) {
					glVertex3f(p[i][j][k][0], p[i][j][k][1], p[i][j][k][2]);
					glVertex3f(p[i+1][j][k][0], p[i+1][j][k][1], p[i+1][j][k][2]);
				}
		glEnd();
	}

}

void MyGLWidget::drawVertex(glm::mat4 model,int vertices1Size, int indices1Size, float* vertexArray, unsigned short* indexArray, vector<Vertex*> verts, bool calledFromFFD){
	float* colors = new float[vertices1Size * 4];
	float* normals = new float[vertices1Size * 4];
	if (calledFromFFD){
		vertices1Size = l*m*n;
	}
	if (!calledFromFFD){
		for(int i=0; i<vertices1Size; i++){
			for (int j=0; j<selectedVertices.size(); j++){
				if (verts[i]->num == selectedVertices[j]){
					colors[i*4]=0;
					colors[i*4+1]=0;
					colors[i*4+2]=1;
					colors[i*4+3]=1;
				}
			}
		}
	}
	else{
		for(int i=0; i<vertices1Size; i++){
			colors[i*4]=0;
			colors[i*4+1]=0;
			colors[i*4+2]=1;
			colors[i*4+3]=1;
		}
		for (int j=0; j<selectedCP.size(); j++) {
			xn = selectedCP[j][0];
			yn = selectedCP[j][1];
			zn = selectedCP[j][2];
			int i = xn*m*n + yn*n + zn;
			colors[i*4]=1;
			colors[i*4+1]=1;
			colors[i*4+2]=0;
			colors[i*4+3]=1;
		}
	}
	
	for(int i=0; i<vertices1Size*4; i=i+4){
		normals[i]=1.0f;
		normals[i+1]=1.0f;
		normals[i+2]=1.0f;
		normals[i+3]=0.0f;
	}
		
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices1Size * 4 * sizeof(float), vertexArray, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_cbo);
	glBufferData(GL_ARRAY_BUFFER, vertices1Size * 4 * sizeof(float), colors, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_nbo);
	glBufferData(GL_ARRAY_BUFFER, vertices1Size * 4 * sizeof(float), normals, GL_STATIC_DRAW); 	

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//now use color data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_cbo);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, vertex_nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//the last thing we need to do is setup our indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices1Size * sizeof(unsigned short), indexArray, GL_STATIC_DRAW);

	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
	glUniform3f(u_lightPosition, 5, 5, 5);

	glPointSize(10.0f);
	glDrawElements(GL_POINTS, indices1Size, GL_UNSIGNED_SHORT, 0);	
	
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);

	delete[] colors;
	delete[] normals;
}

void MyGLWidget::drawVertex1(glm::mat4 model,int vertices1Size, int indices1Size, float* vertexArray, unsigned short* indexArray){

	float* colors = new float[vertices1Size * 4];
	float* normals = new float[vertices1Size * 4];

	for(int i=0; i<vertices1Size; i++){
		for (int j=0; j<selectedVertices.size(); j++){
			if (i == selectedVertices[j]){
				colors[i*4]=0;
				colors[i*4+1]=0;
				colors[i*4+2]=1;
			}
			else{
				colors[i*4]=0;
				colors[i*4+1]=0;
				colors[i*4+2]=1;
			}
			colors[i*4+3]=1;
		}
	}
	
	for(int i=0; i<vertices1Size*4; i=i+4){
		normals[i]=0.0f;
		normals[i+1]=0.0f;
		normals[i+2]=1.0f;
		normals[i+3]=0.0f;
	}
		
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices1Size * 4 * sizeof(float), vertexArray, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_nbo);
	glBufferData(GL_ARRAY_BUFFER, vertices1Size * 4 * sizeof(float), normals, GL_STATIC_DRAW); 

	glBindBuffer(GL_ARRAY_BUFFER, vertex_cbo);
	glBufferData(GL_ARRAY_BUFFER, vertices1Size * 4 * sizeof(float), colors, GL_STATIC_DRAW);	

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//now use color data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_cbo);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, vertex_nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//the last thing we need to do is setup our indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices1Size * sizeof(unsigned short), indexArray, GL_STATIC_DRAW);

	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
	glUniform3f(u_lightPosition, 5, 5, 5);

	glPointSize(10.0f);
	glDrawElements(GL_POINTS, indices1Size, GL_UNSIGNED_SHORT, 0);	
	
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);

	delete[] colors;
	delete[] normals;
}

void MyGLWidget::drawWireframe(glm::mat4 model){
		ind = 1;
		int size = mesh->getFSize();
		for (int cur=0; cur<size; cur++){
			int edgeIndex = -1;
			Face* f = mesh->getF(cur);
			vector<Vertex*> vertices1 = vector<Vertex*>();
			vector<int> indices1 = vector<int>();

			HalfEdge* he = f->he;
			for (int j=0; j<f->verticesCount(); j++) {
				vertices1.push_back(he->v);
				if (he->id == selectedEdge){
					edgeIndex = j;
				}
				he = he->next;
			}

			numPoints = vertices1.size();
						float* normals = new float[numPoints * 4];
			float* colors = new float[numPoints * 4];
		
			int c = 0;
			int n = 0;
			ind = 1;


			f->index = n*4;
			float* vertices = new float[numPoints * 4];
			for (int j=0; j<numPoints; j++) {
				glm::vec3 v = vertices1[j]->toVec();
				vertices[j*4] = v[0];
				vertices[j*4+1] = v[1];
				vertices[j*4+2] = v[2];
				vertices[j*4+3] = 1.0f;

				normals[n*4] = 1;
				normals[n*4+1] = 1;
				normals[n*4+2] = 1;
				normals[n*4+3] = 0.0f;
				n++;

				indices1.push_back(j);

				colors[c*4] = 0;
				colors[c*4+1] = 0;
				colors[c*4+2] = 1;
				colors[c*4+3] = 1.0f;
					
				c++; //haha get it
			}

			numInds=indices1.size();
			unsigned short* indices = new unsigned short[indices1.size()];
			for (int j=0; j<indices1.size(); j++){
				indices[j] = (unsigned short)indices1[j];
			}
			indexArray=indices;
			vertices1.clear();
			indices1.clear();


		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), vertices, GL_STATIC_DRAW);
	
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), normals, GL_STATIC_DRAW); 
	
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
		glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), colors, GL_STATIC_DRAW);	
	
		//activate our three kinds of information
		glEnableVertexAttribArray(positionLocation);
		glEnableVertexAttribArray(colorLocation);
		glEnableVertexAttribArray(normalLocation);
	
		//we're using the vertex data first
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
		glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
		
		//now use color data
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glVertexAttribPointer(colorLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
		//one more time with the normals
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
		//the last thing we need to do is setup our indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInds * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	
		//set the model uniform
		//GL_FALSE is used for not doing transpose operation, because glm has already provided us column major matrices.
		//if the matrices you used is row major, change this parameter to GL_TRUE.
		glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
		glUniform3f(u_lightPosition, 5, 5, 5);
	
		//draw the elements
		glLineWidth(2.0f);
		glDrawElements(GL_LINE_LOOP, numInds, GL_UNSIGNED_SHORT, 0);	
		
		//shut off the information since we're done drawing
		glDisableVertexAttribArray(positionLocation);
		glDisableVertexAttribArray(colorLocation);
		glDisableVertexAttribArray(normalLocation);
	
		//drawVertex(model, numPoints, numInds, vertices, indices, vertices1);
		delete[] vertices;
		delete[] colors;
		delete[] normals;
		delete[] indices;
	}

		ind = 1;
		size = selectedEdges.size();
		for (int cur=0; cur<size + 1; cur++){
			int edgeIndex = -1;
			vector<Vertex*> vertices1 = vector<Vertex*>();
			vector<int> indices1 = vector<int>();

			HalfEdge* he;
			if (cur<size){
				he = mesh->getHE(selectedEdges[cur]);
			}
			else if (edgeLoopEdge != -1){
				he = mesh->getHE(edgeLoopEdge);
			}
			else{
				break;
			}
			HalfEdge* firstHE = he;
			vertices1.push_back(he->v);

			do{
				he = he->next;
			}while(he->next != firstHE);

			vertices1.push_back(he->v);

			numPoints = 2;
			float* normals = new float[numPoints * 4];
			float* colors = new float[numPoints * 4];
		
			int c = 0;
			int n = 0;
			ind = 1;

			float* vertices = new float[numPoints * 4];
			for (int j=0; j<numPoints; j++) {
				glm::vec3 v = vertices1[j]->toVec();
				vertices[j*4] = v[0];
				vertices[j*4+1] = v[1];
				vertices[j*4+2] = v[2];
				vertices[j*4+3] = 1.0f;

				normals[n*4] = 1;
				normals[n*4+1] = 1;
				normals[n*4+2] = 1;
				normals[n*4+3] = 0.0f;
				n++;

				indices1.push_back(j);

				colors[c*4] = 1.0;
				colors[c*4+1] = 1.0;
				colors[c*4+2] = 1.0;
				colors[c*4+3] = 1.0f;
					
				c++; //haha get it
			}

			numInds=indices1.size();
			unsigned short* indices = new unsigned short[indices1.size()];
			for (int j=0; j<indices1.size(); j++){
				indices[j] = (unsigned short)indices1[j];
			}
			indexArray=indices;
			vertices1.clear();
			indices1.clear();


		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), vertices, GL_STATIC_DRAW);
	
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), normals, GL_STATIC_DRAW); 
	
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
		glBufferData(GL_ARRAY_BUFFER, numPoints * 4 * sizeof(float), colors, GL_STATIC_DRAW);	
	
		//activate our three kinds of information
		glEnableVertexAttribArray(positionLocation);
		glEnableVertexAttribArray(colorLocation);
		glEnableVertexAttribArray(normalLocation);
	
		//we're using the vertex data first
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
		glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
		
		//now use color data
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glVertexAttribPointer(colorLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
		//one more time with the normals
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
		//the last thing we need to do is setup our indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInds * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	
		//set the model uniform
		//GL_FALSE is used for not doing transpose operation, because glm has already provided us column major matrices.
		//if the matrices you used is row major, change this parameter to GL_TRUE.
		glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
		glUniform3f(u_lightPosition, 5, 5, 5);
	
		//draw the elements
		glLineWidth(2.0f);
		glDrawElements(GL_LINES, numInds, GL_UNSIGNED_SHORT, 0);	
		
		//shut off the information since we're done drawing
		glDisableVertexAttribArray(positionLocation);
		glDisableVertexAttribArray(colorLocation);
		glDisableVertexAttribArray(normalLocation);
	
		//drawVertex(model, numPoints, numInds, vertices, indices, vertices1);
		delete[] vertices;
		delete[] colors;
		delete[] normals;
		delete[] indices;
		}
}

void MyGLWidget::drawGrid(glm::mat4 model){

	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for(int i=0;i<=10;i++) {
		glVertex3f(i-5,0,-5);
		glVertex3f(i-5,0,5);
		glVertex3f(-5,0,i-5);
		glVertex3f(5,0,i-5);
	}
	glEnd();

}

void MyGLWidget::drawRay(){
		glBegin(GL_LINES);
		glVertex3f(lastCam[0], lastCam[1], lastCam[2]);
		glVertex3f(rayPos[0],rayPos[1],rayPos[2]);
		glEnd();
}

void MyGLWidget::resizeGL(int width, int height) {
	//set the viewport, more boilerplate
	glViewport(0, 0, width, height);
	glm::mat4 proj;
	if(cameraType==ORTHO)
		proj=arcballCamera->getProjMatrix(cameraType)*arcballCamera->getViewMatrix(QUAT);
	else
		proj=arcballCamera->getProjMatrix(cameraType)*arcballCamera->getViewMatrix(cameraType);
	//set the projection matrix here, it only needs to be changed if the screen is resized otherwise it can stay the same
	glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &proj[0][0]);
}

void MyGLWidget::unitCube() {
	mesh = new Mesh();
	Vertex* v0 = new Vertex(1,1,1,0);
	Vertex* v1 = new Vertex(-1,1,1,1);
	Vertex* v2 = new Vertex(-1,-1,1,2);
	Vertex* v3 = new Vertex(1,-1,1,3);
	Vertex* v4 = new Vertex(1,-1,-1,4);
	Vertex* v5 = new Vertex(1,1,-1,5);
	Vertex* v6 = new Vertex(-1,1,-1,6);
	Vertex* v7 = new Vertex(-1,-1,-1,7);
	v0->isOrig=true;
	v1->isOrig=true;
	v2->isOrig=true;	
	v3->isOrig=true;
	v4->isOrig=true;
	v5->isOrig=true;
	v6->isOrig=true;
	v7->isOrig=true;
	float c0[3] = {1,1,0};
	float c1[3] = {1,0,0};
	float c2[3] = {1,0,1};
	float c3[3] = {0,0,1};
	float c4[3] = {0,1,1};
	float c5[3] = {0,1,0};
	// face v0-v1-v2-v3
	HalfEdge* e0 = new HalfEdge(v1,0);
	HalfEdge* e1 = new HalfEdge(v2,1);
	HalfEdge* e2 = new HalfEdge(v3,2);
	HalfEdge* e3 = new HalfEdge(v0,3);
	e0->next = e1;
	e1->next = e2;
	e2->next = e3;
	e3->next = e0;
	Face* f0 = new Face(e0,c0,0);
	e0->f = f0;
	e1->f = f0;
	e2->f = f0;
	e3->f = f0;
	// face v0-v3-v4-v5
	HalfEdge* e4 = new HalfEdge(v3,4);
	HalfEdge* e5 = new HalfEdge(v4,5);
	HalfEdge* e6 = new HalfEdge(v5,6);
	HalfEdge* e7 = new HalfEdge(v0,7);
	e4->next = e5;
	e5->next = e6;
	e6->next = e7;
	e7->next = e4;
	Face* f1 = new Face(e4,c1,1);
	e4->f = f1;
	e5->f = f1;
	e6->f = f1;
	e7->f = f1;
	// face v0-v5-v6-v1
	HalfEdge* e8 = new HalfEdge(v5,8);
	HalfEdge* e9 = new HalfEdge(v6,9);
	HalfEdge* e10 = new HalfEdge(v1,10);
	HalfEdge* e11 = new HalfEdge(v0,11);
	e8->next = e9;
	e9->next = e10;
	e10->next = e11;
	e11->next = e8;
	Face* f2 = new Face(e8,c2,2);
	e8->f = f2;
	e9->f = f2;
	e10->f = f2;
	e11->f = f2;
	// face v1-v6-v7-v2
	HalfEdge* e12 = new HalfEdge(v6,12);
	HalfEdge* e13 = new HalfEdge(v7,13);
	HalfEdge* e14 = new HalfEdge(v2,14);
	HalfEdge* e15 = new HalfEdge(v1,15);
	e12->next = e13;
	e13->next = e14;
	e14->next = e15;
	e15->next = e12;
	Face* f3 = new Face(e12,c3,3);
	e12->f = f3;
	e13->f = f3;
	e14->f = f3;
	e15->f = f3;
	// face v7-v4-v3-v2
	HalfEdge* e16 = new HalfEdge(v4,16);
	HalfEdge* e17 = new HalfEdge(v3,17);
	HalfEdge* e18 = new HalfEdge(v2,18);
	HalfEdge* e19 = new HalfEdge(v7,19);
	e16->next = e17;
	e17->next = e18;
	e18->next = e19;
	e19->next = e16;
	Face* f4 = new Face(e16,c4,4);
	e16->f = f4;
	e17->f = f4;
	e18->f = f4;
	e19->f = f4;
	// face v4-v7-v6-v5
	HalfEdge* e20 = new HalfEdge(v7,20);
	HalfEdge* e21 = new HalfEdge(v6,21);
	HalfEdge* e22 = new HalfEdge(v5,22);
	HalfEdge* e23 = new HalfEdge(v4,23);
	e20->next = e21;
	e21->next = e22;
	e22->next = e23;
	e23->next = e20;
	Face* f5 = new Face(e20,c5,5);
	e20->f = f5;
	e21->f = f5;
	e22->f = f5;
	e23->f = f5;
	// assign symetric halfedges
	e0->sym = e11;
	e11->sym = e0;
	e1->sym = e15;
	e15->sym = e1;
	e2->sym = e18;
	e18->sym = e2;
	e3->sym = e4;
	e4->sym = e3;
	e5->sym = e17;
	e17->sym = e5;
	e6->sym = e23;
	e23->sym = e6;
	e7->sym = e8;
	e8->sym = e7;
	e9->sym = e22;
	e22->sym = e9;
	e10->sym = e12;
	e12->sym = e10;
	e13->sym = e21;
	e21->sym = e13;
	e14->sym = e19;
	e19->sym = e14;
	e16->sym = e20;
	e20->sym = e16;
	// add vertices, halfedges and faces to mesh
	mesh->addV(v0);
	mesh->addV(v1);
	mesh->addV(v2);
	mesh->addV(v3);
	mesh->addV(v4);
	mesh->addV(v5);
	mesh->addV(v6);
	mesh->addV(v7);
	mesh->addHE(e0);
	mesh->addHE(e1);
	mesh->addHE(e2);
	mesh->addHE(e3);
	mesh->addHE(e4);
	mesh->addHE(e5);
	mesh->addHE(e6);
	mesh->addHE(e7);
	mesh->addHE(e8);
	mesh->addHE(e9);
	mesh->addHE(e10);
	mesh->addHE(e11);
	mesh->addHE(e12);
	mesh->addHE(e13);
	mesh->addHE(e14);
	mesh->addHE(e15);
	mesh->addHE(e16);
	mesh->addHE(e17);
	mesh->addHE(e18);
	mesh->addHE(e19);
	mesh->addHE(e20);
	mesh->addHE(e21);
	mesh->addHE(e22);
	mesh->addHE(e23);
	mesh->addF(f0);
	mesh->addF(f1);
	mesh->addF(f2);
	mesh->addF(f3);
	mesh->addF(f4);
	mesh->addF(f5);
	mesh->order();
	ffd = false;
	while (points.size() > 0)
		points.pop_back();
	if (mesh->getVSize() > 0) {
		xmin = mesh->getV(0)->x;
		xmax = mesh->getV(0)->x;
		ymin = mesh->getV(0)->y;
		ymax = mesh->getV(0)->y;
		zmin = mesh->getV(0)->z;
		zmax = mesh->getV(0)->z;
		for (int i=1; i<mesh->getVSize(); i++) {
			Vertex* v = mesh->getV(i);
			if (v->x < xmin)
				xmin = v->x;
			if (v->x > xmax)
				xmax = v->x;
			if (v->y < ymin)
				ymin = v->y;
			if (v->y > ymax)
				ymax = v->y;
			if (v->z < zmin)
				zmin = v->z;
			if (v->z > zmax)
				zmax = v->z;
		}
	}
}

void MyGLWidget::makeMesh() {
	bar = new QProgressBar(this);
	bar->resize(120,30);
	bar->move(1031-120,831-30);
	bar->setMinimum(0);
	bar->setMaximum(100);
	bar->setTextVisible(false);
	bar->show();
	int total = _v.size() + _f.size() * 6;
	mesh = new Mesh();
	int hecount = 0;
	float c[3] = {0.8,0.8,0.8};
	if (_v.size() > 0) {
		xmin = _v[0][0];
		xmax = _v[0][0];
		ymin = _v[0][1];
		ymax = _v[0][1];
		zmin = _v[0][2];
		zmax = _v[0][2];
	}
	for (int i=0; i<_v.size(); i++) {
		glm::vec3 v1 = _v[i];
		Vertex* v0 = new Vertex(v1[0], v1[1], v1[2], i);
		v0->isOrig=true;
		mesh->addV(v0);
		if (v1[0] < xmin)
			xmin = v1[0];
		if (v1[0] > xmax)
			xmax = v1[0];
		if (v1[1] < ymin)
			ymin = v1[1];
		if (v1[1] > ymax)
			ymax = v1[1];
		if (v1[2] < zmin)
			zmin = v1[2];
		if (v1[2] > zmax)
			zmax = v1[2];
		bar->setValue(((i+1)*100)/total);
	}
	int vsize = _v.size();
	map<pair<int,int>, HalfEdge*> edgemap = map<pair<int,int>, HalfEdge*>();
	for (int i=0; i<_f.size(); i++) {
		Face* f0 = new Face(c, i);
		mesh->addF(f0);
		if (_f[i][3] == -1 || abs(_f[i][3] + 1) <= FLT_EPSILON) {
			HalfEdge* he0 = new HalfEdge(mesh->getV(_f[i][1]), hecount);
			hecount++;
			HalfEdge* he1 = new HalfEdge(mesh->getV(_f[i][2]), hecount);
			hecount++;
			HalfEdge* he2 = new HalfEdge(mesh->getV(_f[i][0]), hecount);
			hecount++;
			mesh->addHE(he0);
			mesh->addHE(he1);
			mesh->addHE(he2);
			f0->he = he0;
			he0->next = he1;
			he0->f = f0;
			he1->next = he2;
			he1->f = f0;
			he2->next = he0;
			he2->f = f0;
			map<pair<int,int>, HalfEdge*>::iterator it = edgemap.find(pair<int,int>(_f[i][1],_f[i][0]));
			if (it != edgemap.end()) {
				HalfEdge* hes = it->second;
				he0->sym = hes;
				hes->sym = he0;
			}
			else {
				edgemap.insert(pair<pair<int,int>, HalfEdge*>(pair<int,int>(_f[i][0],_f[i][1]), he0));
				he0->sym = 0;
			}
			it = edgemap.find(pair<int,int>(_f[i][2],_f[i][1]));
			if (it != edgemap.end()) {
				HalfEdge* hes = it->second;
				he1->sym = hes;
				hes->sym = he1;
			}
			else {
				edgemap.insert(pair<pair<int,int>, HalfEdge*>(pair<int,int>(_f[i][1],_f[i][2]), he1));
				he1->sym = 0;
			}
			it = edgemap.find(pair<int,int>(_f[i][0],_f[i][2]));
			if (it != edgemap.end()) {
				HalfEdge* hes = it->second;
				he2->sym = hes;
				hes->sym = he2;
			}
			else {
				edgemap.insert(pair<pair<int,int>, HalfEdge*>(pair<int,int>(_f[i][2],_f[i][0]), he2));
				he2->sym = 0;
			}
		}
		else {
			HalfEdge* he0 = new HalfEdge(mesh->getV(_f[i][1]), hecount);
			hecount++;
			HalfEdge* he1 = new HalfEdge(mesh->getV(_f[i][2]), hecount);
			hecount++;
			HalfEdge* he2 = new HalfEdge(mesh->getV(_f[i][3]), hecount);
			hecount++;
			HalfEdge* he3 = new HalfEdge(mesh->getV(_f[i][0]), hecount);
			hecount++;
			mesh->addHE(he0);
			mesh->addHE(he1);
			mesh->addHE(he2);
			mesh->addHE(he3);
			f0->he = he0;
			he0->next = he1;
			he0->f = f0;
			he1->next = he2;
			he1->f = f0;
			he2->next = he3;
			he2->f = f0;
			he3->next = he0;
			he3->f = f0;
			map<pair<int,int>, HalfEdge*>::iterator it = edgemap.find(pair<int,int>(_f[i][1],_f[i][0]));
			if (it != edgemap.end()) {
				HalfEdge* hes = it->second;
				he0->sym = hes;
				hes->sym = he0;
			}
			else {
				edgemap.insert(pair<pair<int,int>, HalfEdge*>(pair<int,int>(_f[i][0],_f[i][1]), he0));
				he0->sym = 0;
			}
			it = edgemap.find(pair<int,int>(_f[i][2],_f[i][1]));
			if (it != edgemap.end()) {
				HalfEdge* hes = it->second;
				he1->sym = hes;
				hes->sym = he1;
			}
			else {
				edgemap.insert(pair<pair<int,int>, HalfEdge*>(pair<int,int>(_f[i][1],_f[i][2]), he1));
				he1->sym = 0;
			}
			it = edgemap.find(pair<int,int>(_f[i][3],_f[i][2]));
			if (it != edgemap.end()) {
				HalfEdge* hes = it->second;
				he2->sym = hes;
				hes->sym = he2;
			}
			else {
				edgemap.insert(pair<pair<int,int>, HalfEdge*>(pair<int,int>(_f[i][2],_f[i][3]), he2));
				he2->sym = 0;
			}
			it = edgemap.find(pair<int,int>(_f[i][0],_f[i][3]));
			if (it != edgemap.end()) {
				HalfEdge* hes = it->second;
				he3->sym = hes;
				hes->sym = he3;
			}
			else {
				edgemap.insert(pair<pair<int,int>, HalfEdge*>(pair<int,int>(_f[i][3],_f[i][0]), he3));
				he3->sym = 0;
			}
		}
		bar->setValue(((vsize+(1+i)*5)*100)/total);
	}
	mesh->mname = mname;
	cout<<bar->value()<<endl;
	if (mesh->mname)
			readMaterial(mesh->mname);
		else
			readMaterial("default.mtl");
	mesh->order();
	ffd = false;
	while (points.size() > 0)
		points.pop_back();
	delete bar;
	bar = 0;
}

void MyGLWidget::keyPressEvent(QKeyEvent *e){
	//a good place to process keystrokes
	if (e->key() == Qt::Key_Alt)
		keyPressed = Qt::Key_Alt;
	else if (e->key() == Qt::Key_Shift)
		keyPressed = Qt::Key_Shift;
	else if(e->key()==Qt::Key_Q){
		if(cameraType==DUMMY){
			cameraType=QUAT;
			resizeGL(width(), height());
		}
		else if(cameraType==QUAT){
			cameraType=DUMMY;
		}
	}
	else if(e->key()==Qt::Key_W){
		drawFrame = !drawFrame;
	}
	else if(e->key()==Qt::Key_R){
		reset();
	}
	else if(e->key()==Qt::Key_S){
		smooth();
	}
	else if(e->key()==Qt::Key_Space){
		fast = !fast;
	}
	else{
		keyPressed = -1;
	}
	rightReleaseSignal();
	updateGL();
}

void MyGLWidget::keyReleaseEvent(QKeyEvent* e) {
	keyPressed = -1;
	rightReleaseSignal();
}

void MyGLWidget::mousePressEvent(QMouseEvent* e) {
	mouseX = e->x();
	mouseY = e->y();
	//oldMouseX=e->x();
	//oldMouseY=e->y();
	mouseClicked=true;
	if (e->button() == Qt::LeftButton) {
		rightReleaseSignal();		//This tells the viewer to hide the display buttons
		if (keyPressed == -1) {
			if (currentMode == CP && ffd) {
				selectedFaces.clear();
				mousePressed = true;
				mouseButton = 2;
				oldPoint = getWorldCoords(e);
				selectCP(e);
			}
			else {
				if (currentAction==EDGELOOP && edgeLoopEdge != -1){
					HalfEdge* el = mesh->getHE(edgeLoopEdge);
					do{
						el = el->next;
					}while(el->next != mesh->getHE(edgeLoopEdge));
					mesh->insertEdgeLoop(el);
					edgeLoopEdge = -1;
				}
				selectedFaces.clear();
				mousePressed = true;
				mouseButton = 2;
				oldPoint = getWorldCoords(e);
				mouseInPoly(e);
			}
		}
		else if (keyPressed == Qt::Key_Alt) {
			mouseButton = 0;
		}
		else if (keyPressed == Qt::Key_Shift){
			if (currentMode == CP && ffd) {
				selectedFaces.clear();
				mousePressed = true;
				mouseButton = 2;
				oldPoint = getWorldCoords(e);
				selectCP(e);
			}
			else {
				mousePressed = true;
				mouseButton = 2;
				oldPoint = getWorldCoords(e);
				mouseInPoly(e);
			}
		}
	}
	else if (e->button() == Qt::RightButton) {
		if (keyPressed == Qt::Key_Alt)
			mouseButton = 1;
		else{
			rightClickSignal(e->x(), e->y(), currentMode);
		}
	}
	mousePressed=false;
	updateGL();
}

void MyGLWidget::mouseDoubleClickEvent(QMouseEvent* e){
	cout<<"fuck yeah"<<endl;
	mouseClicked=true;
	if (e->button() == Qt::LeftButton) {
		if (keyPressed == -1) {
			selectedFaces.clear();
			selectedEdges.clear();
			selectedVertices.clear();
			mousePressed = true;
			mouseButton = 2;
			oldPoint = getWorldCoords(e);
			mouseInPoly(e);
		}
		else if (keyPressed == Qt::Key_Shift){
			mousePressed = true;
			mouseButton = 2;
			oldPoint = getWorldCoords(e);
			mouseInPoly(e);
		}
	}
	rightReleaseSignal();		//This tells the viewer to hide the display buttons
	mousePressed=false;

	if (selectedEdges.size() > 0){
		HalfEdge* current = mesh->getHE(selectedEdges[0]);
		HalfEdge* original = mesh->getHE(selectedEdges[0]);

		selectedEdges.clear();

		do{

			selectedEdges.push_back(current->id);

			current = current->next->sym->next;
		}while(current!=original);
	}

	updateGL();
}

void MyGLWidget::selectCP(QMouseEvent* e) {
	if (keyPressed != Qt::Key_Shift){
		selectedCP.clear();
	}
	P0 =getWorldCoords(e);
	if(mousePressed){
		std::cout << "Someone clicked " << e->x() << ", " <<e->y() << std::endl;
		std::cout << "The point is " <<P0[0]<<", "<<P0[1]<<", "<<P0[2]<< std::endl;
	}
	float dis[5][5][5];

	glm::vec3 P1=arcballCamera->getDummyCam();
	glm::vec3 P2=glm::vec3(P0[0],P0[1],P0[2]);
	glm::vec3 ref = arcballCamera->getRef();

	for (int i=0; i<l; i++)
		for (int j=0; j<m; j++)
			for (int k=0; k<n; k++) {
				dis[i][j][k] = glm::length(glm::cross(P2-P1, P1-p[i][j][k]))/glm::length(P2-P1);
			}

	xn = -1;
	yn = -1;
	zn = -1;
	float dmin = -1.0f;
	for (int i=0; i<l; i++)
		for (int j=0; j<m; j++)
			for (int k=0; k<n; k++)
				if (dis[i][j][k] < 0.15f) {
					float d = glm::dot(p[i][j][k]-P1, ref-P1)/glm::length(ref-P1);
					if (d > 0) {
						if (abs(dmin+1) <= FLT_EPSILON) {
							dmin = d;
							xn = i;
							yn = j;
							zn = k;
						}
						else if (d < dmin) {
							dmin = d;
							xn = i;
							yn = j;
							zn = k;
						}
					}
				}
	cout<<"xn "<<xn<<" yn "<<yn<<" zn "<<zn<<endl;
	if (xn!=-1 && yn!=-1 && zn!=-1) {
		bool exist = false;
		for (int i=0; i<selectedCP.size(); i++)
			if ((int)selectedCP[i][0] == xn && (int)selectedCP[i][1] == yn && (int)selectedCP[i][2] == zn)
				exist = true;
		if (!exist)
			selectedCP.push_back(glm::vec3(xn,yn,zn));
	}
}

void MyGLWidget::mouseInPoly(QMouseEvent* e){
	// Clicking point on the screen. World Coordinate.
	P0 =getWorldCoords(e);
	if(mousePressed){
		ray = true;
		//std::cout << "Someone clicked " << e->x() << ", " <<e->y() << std::endl;
		//std::cout << "The point is " <<P0[0]<<", "<<P0[1]<<", "<<P0[2]<< std::endl;
	}
	float t=0;
	float tempT=0;	//tempT stores last t to compare t values when 2 faces are hit by ray
	int selectInd=-1;

	glm::vec3 P1=arcballCamera->getDummyCam();
	glm::vec3 P2=glm::vec3(P0[0],P0[1],P0[2]);

	glm::vec3 interP;
	glm::vec3 finalInterP(1000.0f,1000.0f,1000.0f);
	//generalize
	face1=-1;

	for(int i=0; i<mesh->getFSize(); i++){
		glm::vec3 S=glm::vec3(mesh->getF(i)->he->v->x,mesh->getF(i)->he->v->y, mesh->getF(i)->he->v->z);
		glm::vec3 N=mesh->getF(i)->normal();
		float numer=glm::dot(N, (S-P1));
		float denom=glm::dot(N, (P2-P1));
		t=numer/denom;
		interP=P1+t*(P2-P1);		//solve for point of intersection;
		//use point in polygon test for p and plane
		//find which 2 values to project
		int ind0, ind1;
		//xy plane
		if(fabs(interP[0])<=fabs(interP[2])&&fabs(interP[1])<=fabs(interP[2])){
			ind0=0;
			ind1=1;
		}
		//yz plane
		else if(fabs(interP[1])<=fabs(interP[0])&&fabs(interP[2])<=fabs(interP[0])){
			ind0=1;
			ind1=2;
		}
		//xz plane
		else{
			ind0=0;
			ind1=2;
		}
		bool selected=pointInPoly(interP[ind0],interP[ind1],ind0,ind1,i);
		if(selected){
			if(tempT==0||t<tempT){
				finalInterP[0]=interP[0];
				finalInterP[1]=interP[1];
				finalInterP[2]=interP[2];
				tempT=t;
				face1=i;
				}	
			}

		//if vertex selection turned on
		if(mousePressed && currentMode == VERTEX && face1==i){
			selectVertex(interP[ind0],interP[ind1],ind0,ind1, face1);
		}
	
		else if (mousePressed && currentMode == EDGE && face1==i){
			selectEdge(interP[ind0],interP[ind1],ind0,ind1, mesh->getF(face1));
		}

		else if (currentAction == EDGELOOP && face1 == i){
			selectEdge(interP[ind0],interP[ind1],ind0,ind1, mesh->getF(face1));
		}
	}
	if(mousePressed) {
		//face=face1;
		if (face1 != -1 && currentMode == FACE) {
			bool exist = false;
			for (int i=0; i<selectedFaces.size(); i++)
				if (selectedFaces[i] == face1)
					exist = true;
			if (!exist)
				selectedFaces.push_back(face1);
		}
		if (face1==-1){
			selectedFaces.clear();
			selectedVertices.clear();
			selectedEdges.clear();
			edgeLoopEdge = -1;
		}
	}
	if (selectedFaces.size() > 0){
		for (int i=0; i<selectedFaces.size(); i++){
			emit pickFace(selectedFaces[i]);
		}
	}

	if(ray){
		rayPos=E0 + 500.0f*(P0 - E0)/(glm::length(P0 - E0));
		lastCam=arcballCamera->getDummyCam();
	}
}

bool MyGLWidget::pointInPoly(float pt1, float pt2, int ind0, int ind1, int faceInd){
	bool inside=false;
	vector<Vertex*> v;
	HalfEdge* halfE=mesh->getF(faceInd)->he;
	do{
		v.push_back(halfE->v);
		halfE=halfE->next;
	}
	while(halfE!=mesh->getF(faceInd)->he);

	//ignore 3rd dimension
	glm::vec2 p1;
	glm::vec2 p2;
	glm::vec2 point=glm::vec2(pt1, pt2);

	for(int i=0; i<v.size(); i++){
		glm::vec3 tempPoint;
		glm::vec3 tempPoint2;
		if(1+i<v.size()){
			tempPoint=glm::vec3(v[i]->x,v[i]->y,v[i]->z);
			tempPoint2=glm::vec3(v[i+1]->x, v[i+1]->y,v[i+1]->z);
		}
		else{
			tempPoint=glm::vec3(v[i]->x,v[i]->y,v[i]->z);
			tempPoint2=glm::vec3(v[0]->x, v[0]->y,v[0]->z);	
		}
		p1=glm::vec2(tempPoint[ind0], tempPoint[ind1]);
		p2=glm::vec2(tempPoint2[ind0], tempPoint2[ind1]);	
		//use 0 and 1 from here for p1 and p2 since they're vec2
		if(p1[1]>p2[1]){
			//swap
			glm::vec2 temp=p1;
			p1=p2;
			p2=temp;
		}
		if(point[1]>p1[1]){
			if(point[1]<=p2[1]){
				if(p1[1]!=p2[1]){
					if((point[0]-p1[0])*(p2[1]-p1[1])
						-((p2[0]-p1[0])*(point[1]-p1[1]))>0)
							inside=!inside;
				}
			}		
		}
	}

	return inside;
}

void MyGLWidget::selectVertex(float pt1, float pt2, int ind0, int ind1, int faceInd){
	HalfEdge* current = mesh->getF(faceInd)->he;
	if (keyPressed != Qt::Key_Shift){
		selectedVertices.clear();
	}
	do{

		Vertex* v = current->v;

		glm::vec3 point = glm::vec3(v->x, v->y, v->z);

		glm::vec2 d = glm::vec2(pt1 - point[ind0], pt2 - point[ind1]);

		float distance = glm::length(d);

		//cout<<distance<<endl;

		if (distance < 0.15f){
			bool exist = false;
			for (int i=0; i<selectedVertices.size(); i++)
				if (selectedVertices[i] == v->num)
					exist = true;
			if (!exist)
				selectedVertices.push_back(v->num);
		}

		current = current->next;

	}while(current != mesh->getF(faceInd)->he);
}

void MyGLWidget::selectEdge(float pt1, float pt2, int ind1, int ind2,Face* f){
	HalfEdge* current = f->he;
	if (keyPressed != Qt::Key_Shift){
		selectedEdges.clear();
	}
	do{
		glm::vec2 P = glm::vec2(pt1,pt2);
		glm::vec3 a = glm::vec3(current->v->x,current->v->y, current->v->z);
		glm::vec3 b = glm::vec3(current->next->v->x,current->next->v->y, current->next->v->z);

		glm::vec2 A = glm::vec2();
		A[0] = a[ind1];
		A[1] = a[ind2];

		glm::vec2 B = glm::vec2();
		B[0] = b[ind1];
		B[1] = b[ind2];

		//cout<<A[0]<<","<<A[1]<<endl;
		//cout<<B[0]<<","<<B[1]<<endl;

		float width = 0.1f;

		float t = ((P[0] - A[0]) * (B[0] - A[0]) + (P[1] - A[1]) * (B[1] - A[1])) / (glm::length(B-A)*glm::length(B-A));

		if (t>=0 && t<=1){
			float x = A[0] + t * (B[0] - A[0]);
			float y = A[1] + t * (B[1] - A[1]);

			glm::vec2 PIntersect = glm::vec2(x,y);

			glm::vec2 dist = P - PIntersect;

			float distance = glm::length(dist);

			if (distance < width){
				if (mousePressed && currentAction != EDGELOOP){
					bool exist = false;
					for (int i=0; i<selectedEdges.size(); i++)
						if (selectedEdges[i] == current->next->num)
							exist = true;
					if (!exist)
						selectedEdges.push_back(current->next->num);
				}
				else{
					edgeLoopEdge = current->next->num;
				}
			}
		}

		current = current->next;
	}while(current != f->he);
}

glm::vec3 MyGLWidget::getWorldCoords(QMouseEvent* e){
	float x = e->x();
	float y = e->y();
	//std::cout << "Someone clicked " << x << ", " << y << std::endl;
	// change to normalized device coordinates
	x = x / (static_cast<float>(this->width()));
	y = 1.0f - y /(static_cast<float> (this->height()));
	
	//qZoom=glm::translate(glm::mat4(1.0f), glm::vec3(0,0,10.0f));
	E0 =arcballCamera->getDummyCam();			// world coordinate of eye

	glm::vec3 U = arcballCamera->getUp();	//up vector
	
	// Viewing vector
	glm::vec3 center = arcballCamera->getRef();
	glm::vec3 C = center - E0; // implies viewing distance is |C|
	float phi = glm::radians(arcballCamera->getFovy()/2.0f);
	float theta =(static_cast<float>(this->width()))/(static_cast<float>(this->height()))*phi;
	glm::vec3 A = glm::cross(C, U);
	glm::vec3 B = glm::cross(A, C);		// The REAL up vector B = A x C
	glm::vec3 M = E0 + C;				// View Center M = E + C
	glm::vec3 V = B * (1.0f/glm::length(B)) * (glm::length(C)*tan(phi));	// V || B, but on NCD
	glm::vec3 H = A * (1.0f/glm::length(A)) * (glm::length(C)*tan(theta));	// H || A, but on NDC

	// Clicking point on the screen. World Coordinate.
	P0 = M + (2.0f*x -1)*H + (2.0f*y -1)*V;
	
	return P0;

}

void MyGLWidget::mouseMoveEvent(QMouseEvent* e) {
	if (e->buttons() == Qt::RightButton){
		sendMouseEvent(e);
	}
	//If you are in face mode, move the current face
	if (keyPressed != Qt::Key_Alt && e->buttons() == Qt::LeftButton){
		glm::vec3 currentPoint = getWorldCoords(e);
		bool mchanged = false;

		if (currentMode == FACE && selectedFaces.size() > 0){

			if (currentAction == MOVE){
				//for (int i=0; i<1; i++){
				for (int i=0; i<selectedFaces.size(); i++){
					if (selectedFaces.size() > 1){
						int g = selectedFaces.size();
						int h = selectedFaces[0];
						int j = selectedFaces[1];
						int qw = 0;
					}
					Face* f = mesh->getF(selectedFaces[i]);	

					float x = currentPoint[0] - oldPoint[0];
					float y = currentPoint[1] - oldPoint[1];
					float z = currentPoint[2] - oldPoint[2];
					
					mesh->translate(f, glm::vec3(x,y,z));
				}
				mchanged = true;
			}
			else if (currentAction == SCALE){
				for (int i=0; i<selectedFaces.size(); i++){
					Face* f = mesh->getF(selectedFaces[i]);
					
					float sign = currentPoint[0]-oldPoint[0];

					float factor = 0;

					if (sign > 0) factor = 1.1;
					else factor = 0.9f;

					mesh->scale(f,factor);
		
				}
			}
			mchanged = true;
			oldPoint = currentPoint;
	
		}
		else if (currentMode == VERTEX && selectedVertices.size() > 0){
			if (currentAction == MOVE){
				for (int i=0; i<selectedVertices.size(); i++){
					Vertex* v = mesh->getV(selectedVertices[i]);	
	
					float x = currentPoint[0] - oldPoint[0];
					float y = currentPoint[1] - oldPoint[1];
					float z = currentPoint[2] - oldPoint[2];
					
					v->x += x;
					v->y += y;
					v->z += z;
				}
				mchanged = true;
			}
			oldPoint = currentPoint;
		}

		else if (currentMode == CP && selectedCP.size() > 0){
			if (currentAction == MOVE){
				for (int i=0; i<selectedCP.size(); i++) {
					xn = (int)(selectedCP[i][0]);
					yn = (int)(selectedCP[i][1]);
					zn = (int)(selectedCP[i][2]);
					float x = currentPoint[0] - oldPoint[0];
					float y = currentPoint[1] - oldPoint[1];
					float z = currentPoint[2] - oldPoint[2];
				
					p[xn][yn][zn][0] += x;
					p[xn][yn][zn][1] += y;
					p[xn][yn][zn][2] += z;
				}
				deform();
			}
			oldPoint = currentPoint;
		}
		

		else if (currentMode == EDGE && selectedEdges.size() > 0){
			if (currentAction == MOVE){
				for (int i=0; i<selectedEdges.size(); i++){
					Vertex* v = mesh->getHE(selectedEdges[i])->v;	
	
					HalfEdge* current = mesh->getHE(selectedEdges[i]);
					do{
						current=current->next;
					}while(current->next != mesh->getHE(selectedEdges[i]));
	
					float x = currentPoint[0] - oldPoint[0];
					float y = currentPoint[1] - oldPoint[1];
					float z = currentPoint[2] - oldPoint[2];
					
					v->x += x;
					v->y += y;
					v->z += z;
	
					v = current->v;
					v->x += x;
					v->y += y;
					v->z += z;
				mchanged = true;
				}
			}
			oldPoint = currentPoint;
		}
		if (ffd && currentMode != CP && mchanged)
			updatePoints();
		mesh->changed = true;
	}

	//Im guessing this is rotation?
	if (e->buttons()==Qt::LeftButton && keyPressed == Qt::Key_Alt && allowRotation) {
		rotateSignal();
		if(!mouseDragged){
		    if (mouseClicked){
			mouseDragged = true;
			lastRot = thisRot;
			arcballCamera->click(e->x(), e->y());
			}
		}
		else{
			if(mouseClicked){
				getQuatRot(e->x()-oldMouseX, e->y()-oldMouseY);
				arcballCamera->drag(e->x(),e->y(), &quaternion);
				setQuatRotation();
			}
			else
				mouseDragged=false;
		}
		resizeGL(width(),height());
	}
	else if(e->buttons()==Qt::RightButton && keyPressed == Qt::Key_Alt){
		zoomSignal();
		float scale = ((float)(e->x()-mouseX))/this->width();
		if(cameraType==ORTHO)
			arcballCamera->orthZoom(scale/10.0f);
		else
			arcballCamera->zoom(-scale);
		resizeGL(width(),height());
	}
	//panning
	else if(e->buttons()==Qt::MidButton && keyPressed == Qt::Key_Alt){
		float transX =(e->x()-mouseX)/400.0f;
		float transY=(e->y()-mouseY)/400.0f;
		if(panAx==XY)
			arcballCamera->pan(XY, transX, -transY);
		else if(panAx==XZ)
			arcballCamera->pan(XZ, transX, transY);
		else if(panAx==YZ)
			arcballCamera->pan(YZ, -transY, -transX);
		resizeGL(width(),height());
	}
	else if (e->button() == Qt::NoButton) {
		int face2 = face1;
		mouseInPoly(e);	
		//If youre not in face mode, dont highlight the face
		if (face1 != face2) {
			updateGL();
		}
	}
	oldMouseX=e->x();
	oldMouseY=e->y();
	updateGL();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent* e) {
	//cout<<"release"<<e->x()<<" "<<e->y()<<endl;
	mouseButton = 2;
	resetCursor();
	if (e->button()==Qt::RightButton){
		//rightReleaseSignal();
	}
	mouseClicked=false;
	mousePressed=false;
}

void MyGLWidget::getQuatRot(float x, float y){
	float arcballX=-(x/this->width()*360);
	float arcballY=-y/this->height()*360;
	
	arcballCamera->rotateX(arcballX);
	arcballCamera->rotateY(arcballY);
}

void MyGLWidget::setQuatRotation(){
	quaternion=glm::normalize(quaternion);

	thisRot=arcballCamera->setRotation(&quaternion);
	thisRot=thisRot*lastRot;
}

//from swiftless.com
char* MyGLWidget::textFileRead(const char* fileName) {
    char* text = 0;
    
    if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");
        
        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);
            
            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';	//cap off the string with a terminal symbol, fixed by Cory
            }
            fclose(file);
        }
    }
    return text;
}

void MyGLWidget::printLinkInfoLog(int prog) 
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetProgramInfoLog(prog,infoLogLen, &charsWritten, infoLog);
		cerr << "InfoLog:" << endl << infoLog << endl;
		delete [] infoLog;
	}
}

void MyGLWidget::printShaderInfoLog(int shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
		cerr << "InfoLog:" << endl << infoLog << endl;
		delete [] infoLog;
	}

	// should additionally check for OpenGL errors here
}

void MyGLWidget::reloadShader(){
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	
	//Everybody does this
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);

	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//here is stuff for setting up our shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
	
	//load up the source, compile and link the shader program
	const char* vertSource = textFileRead(vertFile);
	const char* fragSource = textFileRead(fragFile);
	glShaderSource(vertexShader, 1, &vertSource, 0);
	glShaderSource(fragmentShader, 1, &fragSource, 0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//For your convenience, i decided to throw in some compiler/linker output helper functions
	//from CIS 565
	GLint compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(vertexShader);
	} 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(fragmentShader);
	} 

	//set the attribute locations for our shaders
	glBindAttribLocation(shaderProgram, positionLocation, "vs_position");
	glBindAttribLocation(shaderProgram, normalLocation, "vs_normal");
	glBindAttribLocation(shaderProgram, colorLocation, "vs_color");

	//finish shader setup
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check for linking success
	GLint linked;
	glGetProgramiv(shaderProgram,GL_LINK_STATUS, &linked);
	if (!linked) 
	{
		printLinkInfoLog(shaderProgram);
	}

	//Get the uniform locations for our shaders, unfortunately they can not be set by us, we have
	u_modelMatrixLocation = glGetUniformLocation(shaderProgram, "u_modelMatrix");
	u_projMatrixLocation = glGetUniformLocation(shaderProgram, "u_projMatrix");
	u_lightPosition=glGetUniformLocation(shaderProgram, "u_lightPosition");
	u_ambient=glGetUniformLocation(shaderProgram, "u_ambient");
	u_camera_position=glGetUniformLocation(shaderProgram, "u_camera_position");
	u_spec=glGetUniformLocation(shaderProgram, "u_specular");
	u_cosine=glGetUniformLocation(shaderProgram, "u_cosine_power");

	//Always remember that it doesn't do much good if you don't have OpenGL actually use the shaders
	glUseProgram(shaderProgram);
}

void MyGLWidget::applyMaterial(){
	mesh->order();
	int total;
	if (bar)
		total = _v.size() + _f.size()*6;
	if(bKd==true){
		for(int i=0; i<mesh->getFSize(); i++){
			mesh->getF(i)->color[0]=Kd[0];
			mesh->getF(i)->color[1]=Kd[1];
			mesh->getF(i)->color[2]=Kd[2];
			if (bar) {
				bar->setValue(bar->value()+((i+1)*100)/total);
			}
		}
	}
	updateGL();
}

//-----------------------SLOTS-----------------------------//

void MyGLWidget::vertexDeleted() {
	//face=-1;
	selectedFaces.clear();
	if (ffd) {
		while (points.size() > 0)
			points.pop_back();
		for (int vi=0; vi<mesh->getVSize(); vi++) {
			Vertex* v = mesh->getV(vi);
			float vs = (v->x - xmin)/(xmax-xmin);
			float vt = (v->y - ymin)/(xmax-xmin);
			float vu = (v->z - zmin)/(xmax-xmin);
			points.push_back(glm::vec3(vs,vt,vu));
		}
	}
	updateGL();
}

void MyGLWidget::start() {
	if (!timer->isActive())
		timer->start(17);
}

void MyGLWidget::pause() {
	timer->stop();
}

void MyGLWidget::stop() {
	timer->stop();
	rotation = 0;
	updateGL();
}

void MyGLWidget::rotate() {
	rotation += 2.5;
	if (rotation >= 360)
		rotation -= 360;
	updateGL();
}
 
void MyGLWidget::updateScene() {
	int size = mesh->getFSize();
	for (int i=0; i<size; i++) {
		Face* f = mesh->getF(i);
		HalfEdge* he = f->checkPlanarity();
		if (he) {
			mesh->splitPolygon(f, he);
			emit quadSplitted();
		}
	}
	mesh->order();
}

void MyGLWidget::reset() {
	timer->stop();
	rotation = 0;
	ray = false;
	mousePressed = false;
	keyPressed = -1;
	face = -1;
	face1 = -1;
	attributes = 0;
	//FFD
	xn = -1;
	yn = -1;
	zn = -1;
	ffd = false;
	gd = false;
	bar = 0;
	edgeLoopEdge=-1;
	selectedFaces.clear();
	selectedVertices.clear();
	selectedEdges.clear();
	//resetCamera();
	resizeGL(width(),height());
	if (meshLoaded)
		makeMesh();
	else
		unitCube();
	sendMeshInfo(mesh);
	updateGL();
}

void MyGLWidget::addEdge(int fa0, int fb0, int hea0, int heb0) {
	if (fa0 == fb0 || hea0 == heb0) {
		cout<<"Invalid add edge input\n";
		return;
	}
	Face* fa = mesh->findF(fa0);
	Face* fb = mesh->findF(fb0);
	HalfEdge* hea = mesh->findHE(hea0);
	HalfEdge* heb = mesh->findHE(heb0);
	if (fa==0 || fb==0 || hea==0 || heb==0) {
		cout<<"Invalid add edge input\n";
		return;
	}
	else {
		if (mesh->addEdge(fa, fb, hea, heb)) {
			emit edgeAdded();
			updateGL();
		}
	}
}

void MyGLWidget::loadObj(char* fileName) {
	char* file = textFileRead(fileName);
	_v = vector<glm::vec3>();
	_f = vector<glm::vec4>();
	mname = 0;
	try {
		vector<char*> lines = vector<char*>();
		char* s = strtok(file, "\n");
		while(s) {
			lines.push_back(s);
			s = strtok(NULL, "\n");
		}
		delete s;
		s = 0;
		for (int i=0; i<lines.size(); i++) {
			vector<char*> tokens = vector<char*>();
			char* t = strtok(lines[i], " ");
			if (!strcmp(t, "v")) {
				while(t) {
					tokens.push_back(t);
					t = strtok(NULL, " ");
				}
				delete t;
				t = 0;
				float v0 = atof(tokens[1]);
				float v1 = atof(tokens[2]);
				float v2 = atof(tokens[3]);
				_v.push_back(glm::vec3(v0,v1,v2));
			}
			else if (!strcmp(t, "f")) {
				while(t) {
					tokens.push_back(t);
					t = strtok(NULL, " ");
				}
				delete t;
				t = 0;
				if (tokens.size() > 5) {
					cout<<"mesh has polygons of more than four vertices"<<endl;
					emit error("Mesh has polygons of more than four vertices.");
					return;
				}
				if (tokens.size() == 4) {
					char* in = strtok(tokens[1], "/");
					int f0 = atoi(in) - 1;
					in = strtok(tokens[2], "/");
					int f1 = atoi(in) - 1;
					in = strtok(tokens[3], "/");
					int f2 = atoi(in) - 1;
					_f.push_back(glm::vec4(f0,f1,f2,-1));
				}
				if (tokens.size() == 5) {
					char* in = strtok(tokens[1], "/");
					int f0 = atoi(in) - 1;
					in = strtok(tokens[2], "/");
					int f1 = atoi(in) - 1;
					in = strtok(tokens[3], "/");
					int f2 = atoi(in) - 1;
					in = strtok(tokens[4], "/");
					int f3 = atoi(in) - 1;
					_f.push_back(glm::vec4(f0,f1,f2,f3));
				}
			}
			else if (!strcmp(t, "mtllib")) {
				t = strtok(NULL, "\n");
				mname = t;
			}
			meshLoaded = true;
		}
	} catch (exception e) {
		meshLoaded = false;
		cout<<"Illegal input file."<<endl;
	}
	if (meshLoaded) {
		makeMesh();
		emit sendMeshInfo(mesh);
	}
}

void MyGLWidget::saveObj(char* fileName) {
	ofstream out;
	out.open(fileName);
	if (mname)
		out<<"mtllib "<<mname<<endl;
	for (int i=0; i<mesh->getVSize(); i++) {
		out<<"v "<<mesh->getV(i)->x<<" "<<mesh->getV(i)->y<<" "<<mesh->getV(i)->z<<endl;
	}
	out<<endl;
	if (mname)
		out<<"usemtl initialShadingGroup"<<endl;
	for (int i=0; i<mesh->getFSize(); i++) {
		out<<"f ";
		Face* f = mesh->getF(i);
		HalfEdge* he = f->he;
		out<<he->v->num + 1<<" ";
		he = he->next;
		while (he != f->he) {
			out<<he->v->num + 1<<" ";
			he = he->next;
		}
		out<<endl;
	}
	out.close();
}

void MyGLWidget::smooth(){
	mesh->subdivide();
	if (ffd) {
		xmin = mesh->getV(0)->x;
		xmax = mesh->getV(0)->x;
		ymin = mesh->getV(0)->y;
		ymax = mesh->getV(0)->y;
		zmin = mesh->getV(0)->z;
		zmax = mesh->getV(0)->z;
		for (int i=1; i<mesh->getVSize(); i++) {
			Vertex* v = mesh->getV(i);
			if (v->x < xmin)
				xmin = v->x;
			if (v->x > xmax)
				xmax = v->x;
			if (v->y < ymin)
				ymin = v->y;
			if (v->y > ymax)
				ymax = v->y;
			if (v->z < zmin)
				zmin = v->z;
			if (v->z > zmax)
				zmax = v->z;
		}
		while (points.size() > 0)
			points.pop_back();
		for (int vi=0; vi<mesh->getVSize(); vi++) {
			Vertex* v = mesh->getV(vi);
			float vs = (v->x - xmin)/(xmax-xmin);
			float vt = (v->y - ymin)/(ymax-ymin);
			float vu = (v->z - zmin)/(zmax-zmin);
			points.push_back(glm::vec3(vs,vt,vu));
		}
	}
	updateGL();
	sendMeshInfo(mesh);
}

void MyGLWidget::incSharpness(){
	for (int i=0; i<selectedEdges.size(); i++){
		(mesh->getHE(selectedEdges[i]))->sharpness++;
	}
}
//Do not delete the stuff below
	//	for (int i=0; i<mesh->getFSize(); i++){
	//	Face* f = mesh->getF(i);

	//	HalfEdge* current = f->he;

	//	do{

	//		current->sharpness = 2;
	//		current = current->next;
	//	}while(current != f->he);
	//}

	//if (selectedEdge != -1){
	//	mesh->getHE(selectedEdge)->sharpness+=1;
	//	mesh->getHE(selectedEdge)->sym->sharpness+=1;
	//	cout<<mesh->getHE(selectedEdge)->sharpness<<endl;
	//}

void MyGLWidget::decSharpness(){
	for (int i=0; i<selectedEdges.size(); i++){
		(mesh->getHE(selectedEdges[i]))->sharpness--;
	}
}

void MyGLWidget::splitQuad(){
	if (selectedFaces.size() > 0){
		for (int i=0; i<selectedFaces.size(); i++){
			char* msg = mesh->splitQuad(mesh->getF(selectedFaces[i]));
			if (msg)
				emit error(msg);
		}
	}
}

void MyGLWidget::insertEdge(){
	if (selectedFaces.size() != 2){
		cout<<"Please select two face"<<endl;
		emit error("More or less than two faces are selected.");
	}
	else{
		HalfEdge* he1 = mesh->getF(selectedFaces[0])->he;
		HalfEdge* he2 = mesh->getF(selectedFaces[1])->he;

		HalfEdge* current1 = he1;
		HalfEdge* current2 = he2;

		HalfEdge* input1 = 0;
		HalfEdge* input2 = 0;
		do{

			do{
				if (current1->v == current2->v){
					input1=current1;
					input2=current2;
					break;
				}

				current2 = current2->next;
			}while(current2 != he2);

			if (input1) break;
			current1 = current1->next;
		}while(current1 != he1);

		if (!input1){
			cout<<"faces do not share vertex"<<endl;
			emit error("Selected faces do not share a vertex.");
		}
		else{
			char* msg = mesh->addEdge(mesh->getF(selectedFaces[0]), mesh->getF(selectedFaces[1]), input1, input2);
			if (msg)
				emit error(msg);
		}

	}
}

void MyGLWidget::insertVertex(){
	for (int i=0; i<selectedEdges.size(); i++){
		HalfEdge* current = mesh->getHE(selectedEdges[i]);
		HalfEdge* original = current;

		do{
			current = current->next;
		}while(current->next != original);

		mesh->addVertex(current);
	}
}

void MyGLWidget::enterEdgeMode(){
	currentMode = EDGE;
	selectedFaces.clear();
	//face = -1;
	face1 = -1;
}

void MyGLWidget::enterVertexMode(){
	currentMode = VERTEX;
	selectedFaces.clear();
	//face = -1;
	face1 = -1;
}

void MyGLWidget::enterObjectMode(){
	currentMode = OBJECT;
	selectedFaces.clear();
	//face = -1;
	face1 = -1;
}

void MyGLWidget::enterFaceMode(){
	currentMode = FACE;
}

void MyGLWidget::enterCPMode() {
	currentMode = CP;
}

void MyGLWidget::enterWireframeMode(){
	currentDisplay = WIREFRAME;
	drawFrame=true;
	updateGL();
}

void MyGLWidget::enterSolidMode(){
	currentDisplay = SOLID;
	updateGL();
}

void MyGLWidget::enterScaleMode(){
	currentAction = SCALE;
}

void MyGLWidget::enterMoveMode(){
	currentAction = MOVE;
}

void MyGLWidget::enterLoopMode(){
	drawFrame = true;
	currentAction = EDGELOOP;
}

void MyGLWidget::enterRotateMode(){
	currentAction = ROTATE;
}

void MyGLWidget::extrude(){
	if (selectedFaces.size() > 0){
		for (int i=0; i<selectedFaces.size(); i++){
			mesh->makeExtrusion(mesh->getF(selectedFaces[i]));
		}
	}
}

void MyGLWidget::extrude2(){
	if (selectedFaces.size() > 0){
		for (int i=0; i<selectedFaces.size(); i++){
			mesh->extrude(mesh->getF(selectedFaces[i]));
		}
	}
}

void MyGLWidget::deleteVertex(){
	if (selectedVertices.size() > 0){
		Vertex* v = mesh->getV(selectedVertices[0]);
		mesh->deleteVertex(v->he);
	}
}

void MyGLWidget::freeFormDeform() {
	if (l>=2 && l<=5 && m>=2 && m<=5 && n>=2 && n<=5) {
		for (int i=0; i<mesh->getVSize(); i++) {
			Vertex* v = mesh->getV(i);
			if (v->x < xmin)
				xmin = v->x;
			if (v->x > xmax)
				xmax = v->x;
			if (v->y < ymin)
				ymin = v->y;
			if (v->y > ymax)
				ymax = v->y;
			if (v->z < zmin)
				zmin = v->z;
			if (v->z > zmax)
				zmax = v->z;
		}
		if (mesh->getVSize() > 0) {
			ffd = true;
			float s = xmax - xmin;
			float t = ymax - ymin;
			float u = zmax - zmin;
			for (int i=0; i<l; i++)
				for (int j=0; j<m; j++)
					for (int k=0; k<n; k++) {
						p[i][j][k] = glm::vec3(xmin + (float)i/(float)(l-1) * s, ymin + (float)j/(float)(m-1) * t, zmin + (float)k/(float)(n-1) * u);
					}
			while (points.size() > 0)
				points.pop_back();
			for (int vi=0; vi<mesh->getVSize(); vi++) {
				Vertex* v = mesh->getV(vi);
				float vs = (v->x - xmin)/s;
				float vt = (v->y - ymin)/t;
				float vu = (v->z - zmin)/u;
				points.push_back(glm::vec3(vs,vt,vu));
				glm::vec3 sum = glm::vec3(0.0f, 0.0f, 0.0f);
				for (int i=0; i<l; i++) {
					for (int j=0; j<m; j++) {
						for (int k=0; k<n; k++) {
							sum += fact(l-1)/(fact(l-1-i)*fact(i)) * pow(1-vs, l-1-i) * pow(vs, i)
								* fact(m-1)/(fact(m-1-j)*fact(j)) * pow(1-vt, m-1-j) * pow(vt, j)
								* fact(n-1)/(fact(n-1-k)*fact(k)) * pow(1-vu, n-1-k) * pow(vu, k) * p[i][j][k];
						}
					}
				}
				v->x = sum[0];
				v->y = sum[1];
				v->z = sum[2];
			}
		}
		updateGL();
	}
	else
		cout<<"Invalid input value for deformer"<<endl;
}

void MyGLWidget::setL(int i) {
	l = i;
}

void MyGLWidget::setM(int i) {
	m = i;
}

void MyGLWidget::setN(int i) {
	n = i;
}

void MyGLWidget::setPX(int i) {
	if (ffd && i < l) {
		xn = i;
		int x = (int)(p[xn][yn][zn][0] * 20.0f);
		int y = (int)(p[xn][yn][zn][1] * 20.0f);
		int z = (int)(p[xn][yn][zn][2] * 20.0f);
		emit px(x);
		emit py(y);
		emit pz(z);
	}
	else
		cout<<"Invalid control point index"<<endl;
}

void MyGLWidget::setPY(int i) {
	if (ffd && i < m) {
		yn = i;
		int x = (int)(p[xn][yn][zn][0] * 20.0f);
		int y = (int)(p[xn][yn][zn][1] * 20.0f);
		int z = (int)(p[xn][yn][zn][2] * 20.0f);
		emit px(x);
		emit py(y);
		emit pz(z);
	}
	else
		cout<<"Invalid control point index"<<endl;
}

void MyGLWidget::setPZ(int i) {
	if (ffd && i < n) {
		zn = i;
		int x = (int)(p[xn][yn][zn][0] * 20.0f);
		int y = (int)(p[xn][yn][zn][1] * 20.0f);
		int z = (int)(p[xn][yn][zn][2] * 20.0f);
		emit px(x);
		emit py(y);
		emit pz(z);
	}
	else
		cout<<"Invalid control point index"<<endl;
}

void MyGLWidget::movePX(int i) {
	if (ffd && xn<l && yn<m && zn<n && xn>-1 && yn>-1 && zn>-1) {
		p[xn][yn][zn][0] = ((float)i)/20.0f;
		deform();
	}
}

void MyGLWidget::movePY(int i) {
	if (ffd && xn<l && yn<m && zn<n && xn>-1 && yn>-1 && zn>-1) {
		p[xn][yn][zn][1] = ((float)i)/20.0f;
		deform();
	}
}

void MyGLWidget::movePZ(int i) {
	if (ffd && xn<l && yn<m && zn<n && xn>-1 && yn>-1 && zn>-1) {
		p[xn][yn][zn][2] = ((float)i)/20.0f;
		deform();
	}
}

void MyGLWidget::deform() {
	if (ffd) {
		if (mesh->getVSize() > 0) {
			float s = xmax - xmin;
			float t = ymax - ymin;
			float u = zmax - zmin;
			int vsize = mesh->getVSize();
			if (gd) {
				bar = new QProgressBar(this);
				bar->resize(120,30);
				bar->move(1031-120,831-30);
				bar->setMinimum(0);
				bar->setMaximum(100);
				bar->setTextVisible(false);
				bar->show();
			}
			for (int vi=0; vi<mesh->getVSize(); vi++) {
				Vertex* v = mesh->getV(vi);
				float vs = points[vi][0];
				float vt = points[vi][1];
				float vu = points[vi][2];
				glm::vec3 sum = glm::vec3(0.0f, 0.0f, 0.0f);
				for (int i=0; i<l; i++) {
					for (int j=0; j<m; j++) {
						for (int k=0; k<n; k++) {
							sum += fact(l-1)/(fact(l-1-i)*fact(i)) * pow(1-vs, l-1-i) * pow(vs, i)
								* fact(m-1)/(fact(m-1-j)*fact(j)) * pow(1-vt, m-1-j) * pow(vt, j)
								* fact(n-1)/(fact(n-1-k)*fact(k)) * pow(1-vu, n-1-k) * pow(vu, k) * p[i][j][k];
						}
					}
				}
				v->x = sum[0];
				v->y = sum[1];
				v->z = sum[2];
				if (bar) {
					bar->setValue(((vi+1)*100)/vsize);
				}
			}
			delete bar;
			bar = 0;
			gd = false;
			mesh->changed = true;
			updateGL();
		}
	}
}

int MyGLWidget::fact(int t) {
	int f = 1;
	for (int i=1; i<=t; i++)
		f = f * i;
	return f;
}

void MyGLWidget::twist() {
	gd = true;
	if (ffd) {
		for (int i=0; i<l; i++)
			for (int j=0; j<m; j++)
				for (int k=0; k<n; k++) {
					if (deformx) {
						float theta = p[i][j][k][0] * twrate/30.0f;
						p[i][j][k][1] = p[i][j][k][1] * cos(theta) - p[i][j][k][2] * sin(theta);
						p[i][j][k][2] = p[i][j][k][1] * sin(theta) + p[i][j][k][2] * cos(theta);
					}
					else if (deformy) {
						float theta = p[i][j][k][1] * twrate/30.0f;
						p[i][j][k][0] = p[i][j][k][0] * cos(theta) - p[i][j][k][2] * sin(theta);
						p[i][j][k][2] = p[i][j][k][0] * sin(theta) + p[i][j][k][2] * cos(theta);
					}
					else if (deformz) {
						float theta = p[i][j][k][2] * twrate/30.0f;
						p[i][j][k][0] = p[i][j][k][0] * cos(theta) - p[i][j][k][1] * sin(theta);
						p[i][j][k][1] = p[i][j][k][0] * sin(theta) + p[i][j][k][1] * cos(theta);
					}
				}
		//mesh->changed = true;
		updateGL();
	}
}

void MyGLWidget::taper() {
	gd = true;
	if (ffd){
		if (tarate <= FLT_EPSILON) {
			cout<<"invalid taper value"<<endl;
			emit error("Invalid taper value.");
			return;
		}
		float pxmin = p[0][0][0][0];
		float pxmax = p[0][0][0][0];
		float pymin = p[0][0][0][1];
		float pymax = p[0][0][0][1];
		float pzmin = p[0][0][0][2];
		float pzmax = p[0][0][0][2];
		for (int i=0; i<l; i++)
			for (int j=0; j<m; j++)
				for (int k=0; k<n; k++) {
					if (p[i][j][k][0] < pxmin)
						pxmin = p[i][j][k][0];
					if (p[i][j][k][0] > pxmax)
						pxmax = p[i][j][k][0];
					if (p[i][j][k][1] < pymin)
						pymin = p[i][j][k][1];
					if (p[i][j][k][1] > pymax)
						pymax = p[i][j][k][1];
					if (p[i][j][k][2] < pzmin)
						pzmin = p[i][j][k][2];
					if (p[i][j][k][2] > pzmax)
						pzmax = p[i][j][k][2];
				}
		for (int i=0; i<l; i++)
			for (int j=0; j<m; j++)
				for (int k=0; k<n; k++) {
					if (deformx) {
						if (pxmax - pxmin <= FLT_EPSILON) {
							cout<<"cannot taper along x axis"<<endl;
							emit error("Cannot taper along x axis.");
							return;
						}
						float r = (p[i][j][k][0] - pxmin) * tarate/(pxmax - pxmin) + 0.2;
						p[i][j][k][1] = p[i][j][k][1] * r;
						p[i][j][k][2] = p[i][j][k][2] * r;
					}
					else if (deformy) {
						if (pymax - pymin <= FLT_EPSILON) {
							cout<<"cannot taper along y axis"<<endl;
							emit error("Cannot taper along y axis.");
							return;
						}
						float r = (p[i][j][k][1] - pymin) * tarate/(pymax - pymin) + 0.2;
						p[i][j][k][0] = p[i][j][k][0] * r;
						p[i][j][k][2] = p[i][j][k][2] * r;
					}
					else if (deformz) {
						if (pzmax - pzmin <= FLT_EPSILON) {
							cout<<"cannot taper along z axis"<<endl;
							emit error("Cannot taper along z axis.");
							return;
						}
						float r = (p[i][j][k][2] - pzmin) * tarate/(pzmax - pzmin) + 0.2;
						p[i][j][k][1] = p[i][j][k][1] * r;
						p[i][j][k][0] = p[i][j][k][0] * r;
					}
				}
		updateGL();
	}
}

void MyGLWidget::updatePoints() {
	xmin = mesh->getV(0)->x;
	xmax = mesh->getV(0)->x;
	ymin = mesh->getV(0)->y;
	ymax = mesh->getV(0)->y;
	zmin = mesh->getV(0)->z;
	zmax = mesh->getV(0)->z;
	for (int i=0; i<mesh->getVSize(); i++) {
		Vertex* v = mesh->getV(i);
		if (v->x < xmin)
			xmin = v->x;
		if (v->x > xmax)
			xmax = v->x;
		if (v->y < ymin)
			ymin = v->y;
		if (v->y > ymax)
			ymax = v->y;
		if (v->z < zmin)
			zmin = v->z;
		if (v->z > zmax)
			zmax = v->z;
	}
	for (int i=0; i<l; i++)
		for (int j=0; j<m; j++)
			for (int k=0; k<n; k++) {
				p[i][j][k] = glm::vec3(xmin + (float)i/(float)(l-1) * (xmax-xmin), ymin + (float)j/(float)(m-1) * (ymax-ymin),
					zmin + (float)k/(float)(n-1) * (zmax-zmin));
			}
	if (mesh->getVSize() == points.size())
		for (int i=0; i<mesh->getVSize(); i++) {
			Vertex* v = mesh->getV(i);
			float vs = (v->x - xmin)/(xmax - xmin);
			float vt = (v->y - ymin)/(ymax - ymin);
			float vu = (v->z - zmin)/(zmax - zmin);
			points[i] = glm::vec3(vs,vt,vu);
		}
	else {
		while (points.size() > 0)
			points.pop_back();
		for (int vi=0; vi<mesh->getVSize(); vi++) {
			Vertex* v = mesh->getV(vi);
			float vs = (v->x - xmin)/(xmax-xmin);
			float vt = (v->y - ymin)/(ymax-ymin);
			float vu = (v->z - zmin)/(zmax-zmin);
			points.push_back(glm::vec3(vs,vt,vu));
		}
	}
}

void MyGLWidget::setTwrate(double t) {
	twrate = t;
}

void MyGLWidget::setTarate(double t) {
	tarate = t;
}

void MyGLWidget::setDX() {
	deformx = true;
	deformy = false;
	deformz = false;
}

void MyGLWidget::setDY() {
	deformx = false;
	deformy = true;
	deformz = false;
}

void MyGLWidget::setDZ() {
	deformx = false;
	deformy = false;
	deformz = true;
}

void MyGLWidget::setBmin(int b) {
	bmin = (float)b/100.0f;
}

void MyGLWidget::setBcen(int b) {
	bcen = (float)b/100.0f;
}

void MyGLWidget::setBmax(int b) {
	bmax = (float)b/100.0f;
}

void MyGLWidget::setCurv(double c) {
	curv = c;
}

void MyGLWidget::bend() {
	gd = true;
	if (ffd) {
		if((bcen-bmin) <= FLT_EPSILON || (bmax-bcen) <= FLT_EPSILON || (bmax-bmin) <= FLT_EPSILON) {
			cout<<"invalid bmin/bcen/bmax input"<<endl;
			emit error("Invalid bmin/bcen/bmax input.");
			return;
		}
		if (curv <= FLT_EPSILON) {
			cout<<"invalid curvature input"<<endl;
			emit error("Invalid curvature input.");
			return;
		}
		float pxmin = p[0][0][0][0];
		float pxmax = p[0][0][0][0];
		float pymin = p[0][0][0][1];
		float pymax = p[0][0][0][1];
		float pzmin = p[0][0][0][2];
		float pzmax = p[0][0][0][2];
		for (int i=0; i<l; i++)
			for (int j=0; j<m; j++)
				for (int k=0; k<n; k++) {
					if (p[i][j][k][0] < pxmin)
						pxmin = p[i][j][k][0];
					if (p[i][j][k][0] > pxmax)
						pxmax = p[i][j][k][0];
					if (p[i][j][k][1] < pymin)
						pymin = p[i][j][k][1];
					if (p[i][j][k][1] > pymax)
						pymax = p[i][j][k][1];
					if (p[i][j][k][2] < pzmin)
						pzmin = p[i][j][k][2];
					if (p[i][j][k][2] > pzmax)
						pzmax = p[i][j][k][2];
				}

		if (deformx) {
			if (pxmax - pxmin <= FLT_EPSILON) {
				cout<<"cannot bend along x axis"<<endl;
				emit error("Cannot bend along x axis.");
				return;
			}
			float bmin1 = bmin * (pxmax - pxmin) + pxmin;
			float bcen1 = bcen * (pxmax - pxmin) + pxmin;
			float bmax1 = bmax * (pxmax - pxmin) + pxmin;
			for (int i=0; i<l; i++)
				for (int j=0; j<m; j++)
					for (int k=0; k<n; k++) {
						if (p[i][j][k][0] - bmin1 <= FLT_EPSILON) {
							float theta = curv * (bmin1 - bcen1);
							p[i][j][k][0] = -sin(theta) * (p[i][j][k][1] - 1.0f/curv) + bcen1 + cos(theta) * (p[i][j][k][0] - bmin1);
							p[i][j][k][1] = cos(theta) * (p[i][j][k][1] - 1.0f/curv) + 1.0f/curv + sin(theta) * (p[i][j][k][0] - bmin1);
						}
						else if (p[i][j][k][0] - bmax1 >= FLT_EPSILON) {
							float theta = curv * (bmax1 - bcen1);
							p[i][j][k][0] = -sin(theta) * (p[i][j][k][1] - 1.0f/curv) + bcen1 + cos(theta) * (p[i][j][k][0] - bmax1);
							p[i][j][k][1] = cos(theta) * (p[i][j][k][1] - 1.0f/curv) + 1.0f/curv + sin(theta) * (p[i][j][k][0] - bmax1);
						}
						else {
							float theta = curv * (p[i][j][k][0] - bcen1);
							p[i][j][k][0] = -sin(theta) * (p[i][j][k][1] - 1.0f/curv) + bcen1;
							p[i][j][k][1] = cos(theta) * (p[i][j][k][1] - 1.0f/curv) + 1.0f/curv;
						}
					}
		}
		else if (deformy) {
			if (pymax - pymin <= FLT_EPSILON) {
				cout<<"cannot bend along y axis"<<endl;
				emit error("Cannot bend along y axis.");
				return;
			}
			float bmin1 = bmin * (pymax - pymin) + pymin;
			float bcen1 = bcen * (pymax - pymin) + pymin;
			float bmax1 = bmax * (pymax - pymin) + pymin;
			for (int i=0; i<l; i++)
				for (int j=0; j<m; j++)
					for (int k=0; k<n; k++) {
						if (p[i][j][k][1] - bmin1 <= FLT_EPSILON) {
							float theta = curv * (bmin1 - bcen1);
							p[i][j][k][1] = -sin(theta) * (p[i][j][k][2] - 1.0f/curv) + bcen1 + cos(theta) * (p[i][j][k][1] - bmin1);
							p[i][j][k][2] = cos(theta) * (p[i][j][k][2] - 1.0f/curv) + 1.0f/curv + sin(theta) * (p[i][j][k][1] - bmin1);
						}
						else if (p[i][j][k][1] - bmax1 >= FLT_EPSILON) {
							float theta = curv * (bmax1 - bcen1);
							p[i][j][k][1] = -sin(theta) * (p[i][j][k][2] - 1.0f/curv) + bcen1 + cos(theta) * (p[i][j][k][1] - bmax1);
							p[i][j][k][2] = cos(theta) * (p[i][j][k][2] - 1.0f/curv) + 1.0f/curv + sin(theta) * (p[i][j][k][1] - bmax1);
						}
						else {
							float theta = curv * (p[i][j][k][1] - bcen1);
							p[i][j][k][1] = -sin(theta) * (p[i][j][k][2] - 1.0f/curv) + bcen1;
							p[i][j][k][2] = cos(theta) * (p[i][j][k][2] - 1.0f/curv) + 1.0f/curv;
						}
					}
		}
		else if (deformz) {
			if (pzmax - pzmin <= FLT_EPSILON) {
				cout<<"cannot bend along z axis"<<endl;
				emit error("Cannot bend along z axis.");
				return;
			}
			float bmin1 = bmin * (pzmax - pzmin) + pzmin;
			float bcen1 = bcen * (pzmax - pzmin) + pzmin;
			float bmax1 = bmax * (pzmax - pzmin) + pzmin;
			for (int i=0; i<l; i++)
				for (int j=0; j<m; j++)
					for (int k=0; k<n; k++) {
						if (p[i][j][k][2] - bmin1 <= FLT_EPSILON) {
							float theta = curv * (bmin1 - bcen1);
							p[i][j][k][2] = -sin(theta) * (p[i][j][k][0] - 1.0f/curv) + bcen1 + cos(theta) * (p[i][j][k][2] - bmin1);
							p[i][j][k][0] = cos(theta) * (p[i][j][k][0] - 1.0f/curv) + 1.0f/curv + sin(theta) * (p[i][j][k][2] - bmin1);
						}
						else if (p[i][j][k][2] - bmax1 >= FLT_EPSILON) {
							float theta = curv * (bmax1 - bcen1);
							p[i][j][k][2] = -sin(theta) * (p[i][j][k][0] - 1.0f/curv) + bcen1 + cos(theta) * (p[i][j][k][2] - bmax1);
							p[i][j][k][0] = cos(theta) * (p[i][j][k][0] - 1.0f/curv) + 1.0f/curv + sin(theta) * (p[i][j][k][2] - bmax1);
						}
						else {
							float theta = curv * (p[i][j][k][2] - bcen1);
							p[i][j][k][2] = -sin(theta) * (p[i][j][k][0] - 1.0f/curv) + bcen1;
							p[i][j][k][0] = cos(theta) * (p[i][j][k][0] - 1.0f/curv) + 1.0f/curv;
						}
					}
		}
	}
	updateGL();
}

void MyGLWidget::snapToVertex(){
	if(selectedVertices.size()>0){
		int ind=selectedVertices[0];
		float pointX=mesh->getV(ind)->x;
		float pointY=mesh->getV(ind)->y;
		float pointZ=mesh->getV(ind)->z;
		arcballCamera->setPan(pointX, pointY, pointZ);
		arcballCamera->setRefPointX(pointX);
		arcballCamera->setRefPointY(pointY);
		arcballCamera->setRefPointZ(pointZ);
		resizeGL(this->width(), this->height());
		updateGL();
	}
}

void MyGLWidget::snapToFace(){
	if(selectedFaces.size()>0){
		int ind=selectedFaces[0];
		glm::vec3 center=mesh->getF(ind)->setCentroid();
		arcballCamera->setPan(center[0], center[1], center[2]);
		arcballCamera->setRefPointX(center[0]);
		arcballCamera->setRefPointY(center[1]);
		arcballCamera->setRefPointZ(center[2]);
		resizeGL(this->width(), this->height());
		updateGL(); 
	}
}

void MyGLWidget::snapToEdge(){
	if(selectedEdges.size()>0){
		HalfEdge* hE=mesh->getHE(selectedEdges[0]);
		Vertex* snapVert1=hE->v;
		HalfEdge* nextE=hE->next;
		while(nextE->next!=hE)
			nextE=nextE->next;
		Vertex* snapVert2=nextE->v;
		float pointX=(snapVert1->x+snapVert2->x)/2.0f;
		float pointY=(snapVert1->y+snapVert2->y)/2.0f;
		float pointZ=(snapVert1->z+snapVert2->z)/2.0f;
		arcballCamera->setRefPointX(pointX);
		arcballCamera->setRefPointY(pointY);
		arcballCamera->setRefPointZ(pointZ);
		arcballCamera->setPan(pointX, pointY, pointZ);
		resizeGL(width(), height());
		updateGL();
	}
}

void MyGLWidget::resetCamera(){
	allowRotation=true;
	arcballCamera->reset();
	quaternion=glm::vec4(0,0,0,0);
	thisRot=glm::mat3(1.0f);
	lastRot=glm::mat3(1.0f);
	cameraType=DUMMY;
	mouseClicked=mouseDragged=false;
	resizeGL(this->width(), this->height());
	updateGL();
}

void MyGLWidget::setCamPersp(){
	panAx=XY;
	allowRotation=true;
	cameraType=DUMMY;
	arcballCamera->setView(PERSP);
	resizeGL(width(), height());
	updateGL();
}

void MyGLWidget::setCamOverhead(){
	panAx=XZ;
	allowRotation=false;
	cameraType=DUMMY;
	arcballCamera->setView(OVERHEAD);
	cameraType=ORTHO;
	resizeGL(width(), height());
	updateGL();
}

void MyGLWidget::setCamFront(){
	panAx=XY;
	allowRotation=false;
	quaternion=glm::vec4(0,0,0,1);
	arcballCamera->setView(FRONT);
	cameraType=ORTHO;
	resizeGL(width(), height());
	updateGL();
}

void MyGLWidget::setCamBack(){
	panAx=XY;
	allowRotation=false;
	quaternion=glm::vec4(0,0,0,1);
	arcballCamera->setView(BACK);
	cameraType=ORTHO;
	resizeGL(width(), height());
	updateGL();
}

void MyGLWidget::setCamLeft(){
	panAx=YZ;
	allowRotation=false;
	quaternion=glm::vec4(0,1,0,0);
	arcballCamera->setView(LEFT);
	cameraType=ORTHO;
	resizeGL(width(), height());
	updateGL();
}

void MyGLWidget::setCamRight(){
	panAx=YZ;
	allowRotation=false;
	quaternion=glm::vec4(0,1,0,0);
	arcballCamera->setView(RIGHT);
	cameraType=ORTHO;
	resizeGL(width(), height());
	updateGL();
}

void MyGLWidget::setCamUserX(int x){
	userCam[0]=x;
	arcballCamera->setDummyCamX(x);
}

void MyGLWidget::setCamUserY(int y){
	userCam[1]=y;
	arcballCamera->setDummyCamY(y);
}

void MyGLWidget::setCamUserZ(int z){
	userCam[2]=z;
	arcballCamera->setDummyCamZ(z);
}

void MyGLWidget::setCamUser(){
	panAx=XY;
	allowRotation=true;
	quaternion=glm::vec4(0,1,0,0);
	cameraType=DUMMY;
	arcballCamera->setView(USER);
	cout<<arcballCamera->getDummyCam()[0]<<" "<<arcballCamera->getDummyCam()[1]<<" "<<arcballCamera->getDummyCam()[2]<<endl;
	resizeGL(width(), height());
	updateGL();
}

void MyGLWidget::createUser(){
	setCamUser();
}

void MyGLWidget::setLightX(int x){
	lightX=x;
	updateGL();
}

void MyGLWidget::setLightY(int y){
	lightY=y;
	updateGL();
}

void MyGLWidget::setLightZ(int z){
	lightZ=z;
	updateGL();
}

void MyGLWidget::loadFragShader(char* fileName){
	fragFile=fileName;
	reloadShader();
	updateGL();
}

void MyGLWidget::loadVertShader(char* fileName){
	vertFile=fileName;
	reloadShader();
	updateGL();
}

void MyGLWidget::readMaterial(char* fileName){
	try {
		char* file = textFileRead(fileName);
		//parses lines
		if (file) {
			vector<char*> lines = vector<char*>();
			char* s = strtok(file, "\n");
			while(s) {
				lines.push_back(s);
				s = strtok(NULL, "\n");
			}
			delete s;
			s = 0;

			vector<char*> tokens = vector<char*>();
			//parse out tokens of material
			for (int i=0; i<lines.size(); i++) {
			char* t = strtok(lines[i], " ");
			while(t) {
				tokens.push_back(t);
				t = strtok(NULL, " ");
			}
			delete t;
			t = 0;
			}
			cout<<endl;
			//read through the tokens and assign variables
			for(int i=0; i<tokens.size(); i++){
				if(i==1){
					mtlName=tokens[i];
				}
				//kd
				if(strcmp(tokens[i],"Kd")==0){
					Kd[0]=atof(tokens[i+1]);
					Kd[1]=atof(tokens[i+2]);
					Kd[2]=atof(tokens[i+3]);
					i=i+3;
					bKd=true;
				}
				else if(strcmp("Ka", tokens[i])==0){
					Ka[0]=atof(tokens[i+1]);
					Ka[1]=atof(tokens[i+2]);
					Ka[2]=atof(tokens[i+3]);
					i=i+3;
					bKa=true;
				}
				else if(strcmp("Ks", tokens[i])==0){
					Ks[0]=atof(tokens[i+1]);
					Ks[1]=atof(tokens[i+2]);
					Ks[2]=atof(tokens[i+3]);
					i=i+3;			
					bKs=true;
				}
				else if(strcmp("Ns", tokens[i])==0){
					Ns=atof(tokens[i+1]);
					bNs=true;
				}
			}
		}
		//else
			//emit error("Invalid material name.");
	} 
	catch (exception e) {
		//emit error("Invalid material name.");
	}

	applyMaterial();
}

void MyGLWidget::setColor(int r, int g, int b){
	Kd[0]=r/255.0f;
	Kd[1]=g/255.0f;
	Kd[2]=b/255.0f;
	bKd=true;
	applyMaterial();
}

void MyGLWidget::setCosPower(double d){
	Ns=d;
	updateGL();
}

void MyGLWidget::setAmbient(int i){
	Ka[0]=Ka[1]=Ka[2]=i/100.0f;
	updateGL();
}

void MyGLWidget::setTransparency(int i){
	Tf[0]=Tf[1]=Tf[2]=i/100.0f;
	Kd[3]=Tf[0];
	updateGL();
}

void MyGLWidget::setSpecCol(int i){
	Ks[0]=Ks[1]=Ks[2]=i/255.0f;
	updateGL();
}

void MyGLWidget::toggleForcePlanarity(){
	forcePlanarity = !forcePlanarity;
}

//for debugging
//void MyGLWidget::drawVertex1(glm::mat4 model,int vertices1Size, int indices1Size, float* vertexArray, unsigned short* indexArray){
//
//	float* colors = new float[vertices1Size * 4];
//	float* normals = new float[vertices1Size * 4];
//
//	for(int i=0; i<vertices1Size; i++){
//		if (i == hoveredVertex&&currentMode==VERTEX) {
//				colors[i] = 0;
//				colors[i+1] = 0;
//				colors[i+2] =1;
//			}
//		//only highlight if vertex selection mode is turned on
//		else if (currentMode==VERTEX) {
//				colors[i] = 1;
//				colors[i+1] = 1;
//				colors[i+2] =1;
//			}
//		else{
//			colors[i*4]=0;
//			colors[i*4+1]=0;
//			colors[i*4+2]=1;
//		}
//		colors[i*4+3]=1;
//	}
//	
//	for(int i=0; i<vertices1Size*4; i=i+4){
//		normals[i]=0.0f;
//		normals[i+1]=0.0f;
//		normals[i+2]=1.0f;
//		normals[i+3]=0.0f;
//	}
//		
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
//	glBufferData(GL_ARRAY_BUFFER, vertices1Size * 4 * sizeof(float), vertexArray, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_nbo);
//	glBufferData(GL_ARRAY_BUFFER, vertices1Size * 4 * sizeof(float), normals, GL_STATIC_DRAW); 
//
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_cbo);
//	glBufferData(GL_ARRAY_BUFFER, vertices1Size * 4 * sizeof(float), colors, GL_STATIC_DRAW);	
//
//	glEnableVertexAttribArray(positionLocation);
//	glEnableVertexAttribArray(colorLocation);
//	glEnableVertexAttribArray(normalLocation);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
//	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
//	
//	//now use color data
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_cbo);
//	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
//
//	//one more time with the normals
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_nbo);
//	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
//
//	//the last thing we need to do is setup our indices
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_ibo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices1Size * sizeof(unsigned short), indexArray, GL_STATIC_DRAW);
//
//	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
//	glUniform3f(u_lightPosition, 5, 5, 5);
//
//	glPointSize(10.0f);
//	glDrawElements(GL_POINTS, indices1Size, GL_UNSIGNED_SHORT, 0);	
//	
//	glDisableVertexAttribArray(positionLocation);
//	glDisableVertexAttribArray(colorLocation);
//	glDisableVertexAttribArray(normalLocation);
//
//	delete[] colors;
//	delete[] normals;
//}