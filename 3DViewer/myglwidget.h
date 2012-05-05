#ifndef MYGLWIDGET
#define MYGLWIDGET

#include "glew.h"
#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include "../glm/glm.hpp"
#include "Mesh.h"
#include <fstream>
#include <math.h>
#include "arcballCam.h"
#include "qprogressbar.h"
#include <qcolor.h>

enum modeType { OBJECT, FACE, EDGE, VERTEX, CP };
enum displayType { WIREFRAME, SOLID };
enum action { EDGELOOP, SCALE, MOVE, ROTATE };

class MyGLWidget : public QGLWidget {
	Q_OBJECT;
private:
	bool drawFrame;
		//Used for when in edge loop mode
	int edgeLoopEdge;
	//vertex arrays needed for drawing
	unsigned int vbo;
	unsigned int cbo;
	unsigned int nbo;
	unsigned int ibo;

	//vertex CBO
	unsigned int vertex_vbo;
	unsigned int vertex_cbo;
	unsigned int vertex_nbo;
	unsigned int vertex_ibo;

//----------------SHADERS---------------//
	//shader file names:
	const char* fragFile;
	const char* vertFile;

	//attributes
	unsigned int positionLocation;
	unsigned int normalLocation;
	unsigned int colorLocation;

	// uniforms
	unsigned int u_projMatrixLocation;
	unsigned int u_modelMatrixLocation;
	
	//needed to compile and link and use the shaders
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;

	//unsigned int u_lightPositionLocation;
	unsigned int u_lightPosition;

	int lightX;
	int lightY;
	int lightZ;

	char* mtlName;
	glm::vec4 Kd;		//diffuse
	glm::vec4 Ka;		//ambient
	glm::vec4 Ks;		//specular
	glm::vec4 Tf;		//transparency
	float Ns;			//cosine power
	//booleans for whether material contains above elements
	bool bKd, bKa, bKs, bNs;

	//shader uniforms for materia
	unsigned int u_ambient;
	unsigned int u_spec;
	unsigned int u_cosine;
	unsigned int u_camera_position;
//----------------SHADERS-----------------//

	//Animation/transformation stuff
	QTimer* timer;

	float rotation;	
	int ind;

	int mouseX;
	int mouseY;
	int mouseButton; // 0 = left, 1 = right
	bool mousePressed;
	int keyPressed;
	
	glm::vec3 E0;
	glm::vec3 P0;
	
	bool ray;
	
	vector<int> selectedFaces;
	vector<int> oldFaces;

	vector<int> selectedVertices;
	vector<int> oldVertices;

	vector<int> selectedEdges;
	vector<int> oldEdges;

	int face;
	int face1;
	
	glm::vec3 E1;
	glm::vec3 P1;
	vector<glm::vec3> _v;
	vector<glm::vec4> _f;
	int attributes;

	glm::vec3 getWorldCoords(QMouseEvent* e);
	//Old coordinates of the mouse
	glm::vec3 oldPoint;

	int oldFace;
	int oldFace1;
	Qt::Key currentButton;

	modeType currentMode;
	displayType currentDisplay;
	action currentAction;

//---------------Camera--------------------//
	arcballCam* arcballCamera;
	glm::mat3 lastRot;
	glm::mat3 thisRot;
	glm::mat4 view;
	glm::vec4 quaternion;
	glm::vec3 userCam;
	float oldMouseX;
	float oldMouseY;
	bool mouseClicked;
	bool mouseDragged;
	bool allowRotation;
	panAxis panAx;
	glm::vec3 rayPos;
	glm::vec3 lastCam;
	camType cameraType;
//---------------END--Camera--------------------//

	float numPoints;
	float numInds;
	float* vertexArray;
	float* normalArray;
	float* colorArray;
	unsigned short* indexArray;

	bool forcePlanarity;
	int selectedEdge;

	//helper function to read shader source and put it in a char array
	//thanks to Swiftless
	char* textFileRead(const char*);

	//some other helper functions from CIS 565
	void printLinkInfoLog(int);
	void printShaderInfoLog(int);

	// FFD
	int l;
	int m;
	int n;
	int xn;
	int yn;
	int zn;
	bool ffd;
	bool gd;
	float xmin, xmax, ymin, ymax, zmin, zmax;
	vector<glm::vec3> points;
	glm::vec3 p[5][5][5]; // control points
	vector<glm::vec3> selectedCP;

	//global deformation
	double twrate;
	double tarate;
	bool deformx;
	bool deformy;
	bool deformz;
	//bend
	double curv;
	float bmin;
	float bmax;
	float bcen;

	//MATERIAL
	char* mname;

	//loadingbar
	QProgressBar* bar;

public:
	MyGLWidget(QWidget*);
	MyGLWidget(QWidget*, Mesh*);
	~MyGLWidget(void);
	void initializeGL(void);
	void paintGL(void);
	void resizeGL(int, int);
	void createCube(glm::mat4);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseDoubleClickEvent(QMouseEvent*);
	void unitCube(); // build a unit cube mesh
	void makeMesh();
	int testIntersection(glm::vec3, glm::vec3);

	//optimized point in poly and mosue movement
	void mouseInPoly(QMouseEvent*);
	bool pointInPoly(float pt1, float pt2, int ind0, int ind1, int faceInd);

	//METHOD FOR DRAWING VERTEX
	void drawVertex(glm::mat4,int vertices1Size, int indices1Size, float*,unsigned short *, vector<Vertex*>, bool calledFromFFD);
	void drawVertex1(glm::mat4,int vertices1Size, int indices1Size, float*,unsigned short *);
	void drawGrid(glm::mat4);
	//Drawing wireframe
	void drawWireframe(glm::mat4);
	void drawRay();

	//methods for selecting vertices and edges
	void selectVertex(float pt1, float pt2, int ind0, int ind1, int faceInd);
	void selectEdge(float pt1, float pt2, int ind1, int ind2, Face* f);

	void setQuatRotation();
	void getQuatRot(float x, float y);

	//load shader
	void reloadShader();
	void applyMaterial();

	int fact(int);		//factorial helper

	//Mesh
	Mesh* mesh;
	bool draw;

	//LOAD OBJ
	bool meshLoaded;
	bool fast;

	void selectCP(QMouseEvent*);

public slots:
	void start();
	void pause();
	void stop();
	void rotate();
	void updateScene();
	void reset();
	void addEdge(int,int,int,int);
	void vertexDeleted();
	void loadObj(char*);
	void saveObj(char*);				
	void smooth();
	void splitQuad();
	void insertEdge();
	void insertVertex();
	void enterFaceMode();
	void enterVertexMode();
	void enterEdgeMode();
	void enterObjectMode();
	void enterWireframeMode();
	void enterSolidMode();
	void enterScaleMode();
	void enterMoveMode();
	void enterLoopMode();
	void enterRotateMode();
	void extrude();
	void extrude2();
	void freeFormDeform();
	void deleteVertex();
	void incSharpness();
	void decSharpness();

	//CP MODE
	void enterCPMode();

	//camera controls
	void snapToVertex();
	void snapToFace();
	void snapToEdge();
	void resetCamera();
	void setCamPersp();
	void setCamOverhead();
	void setCamFront();
	void setCamBack();
	void setCamLeft();
	void setCamRight();
	void setCamUserX(int x);
	void setCamUserY(int y);
	void setCamUserZ(int z);
	void setCamUser();
	void createUser();

	//light controls
	void setLightX(int x);
	void setLightY(int y);
	void setLightZ(int z);

	//shader
	void loadFragShader(char*);
	void loadVertShader(char*);
	void readMaterial(char*);
	void setColor(int, int, int);
	void setCosPower(double d);
	void setAmbient(int i);
	void setTransparency(int i);
	void setSpecCol(int i);

	//FFD
	void setL(int);
	void setM(int);
	void setN(int);
	void setPX(int);
	void setPY(int);
	void setPZ(int);
	void movePX(int);
	void movePY(int);
	void movePZ(int);
	void updatePoints();

	//global deformation
	void twist();
	void taper();
	void setTwrate(double);
	void setTarate(double);
	void setDX();
	void setDY();
	void setDZ();
	void setCurv(double);
	void setBmin(int);
	void setBmax(int);
	void setBcen(int);
	void bend();
	void deform();
	void toggleForcePlanarity();


signals:
	void sendMeshInfo(Mesh*);
	void edgeAdded();
	void quadSplitted();
	void pickFace(int);
	void sendMode(modeType);
	void rightClickSignal(float x, float y, modeType);
	void rightReleaseSignal();
	void initSignal(MyGLWidget*);
	void zoomSignal();
	void rotateSignal();
	void resetCursor();
	void drawn(MyGLWidget*);
	void sendMouseEvent(QMouseEvent* e);
	void px(int);
	void py(int);
	void pz(int);


		//errors
	void error(char*);
	//reset the deformer
	//void untoggle(bool);
};

#endif