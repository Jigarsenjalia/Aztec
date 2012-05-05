#ifndef ARCBALLCAM_H
#define ARCBALLCAM_H

#include <iostream>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/quaternion.hpp"
using namespace std;

enum camType{ORTHO, DUMMY, QUAT};
enum viewType {PERSP, RIGHT, LEFT, FRONT, BACK, OVERHEAD, USER};
enum panAxis{XY, XZ, YZ};

class arcballCam{

private:
	glm::vec3 camPosition;
	glm::vec3 refPosition;
	glm::vec3 up;
	glm::vec3 dummyUp;
	glm::vec3 dummyCam;
	glm::vec3 userCam;

	glm::mat4 qRotation;
	float qZoom;
	glm::vec3 qPan;
	float qOrthZoom;
	
	float screenWidth;
	float screenHeight;
	float fovy;
	float z_near;
	float z_far;
	float aspect;
	float camWidth;
	float camHeight;
	glm::gtc::quaternion::fquat q;
	
public:
	glm::vec3 startVec;
	glm::vec3 endVec;

	arcballCam(const glm::vec3& _ref, const glm::vec3& _cam, const glm::vec3& _up,
		const float _fovy, float width, float height);
	~arcballCam();
	void sphereCoord(float x, float y, glm::vec3*);		//maps screen to hypersphere
	void click(float x, float y);
	void drag(float x, float y, glm::vec4*);
	glm::mat3 setRotation(const glm::vec4*);	//computes rotation matrix from quaternion
	
	glm::mat4 getProjMatrix(camType);
	glm::mat4 getViewMatrix(camType);
	glm::vec3 getCamPos();
	glm::vec3 getRef();
	glm::vec3 getUp();
	glm::vec3 getDummyCam();
	float getAspect();
	float getFovy();

	void setCamPosition(float x, float y, float z);
	void setDummyCamX(float x);
	void setDummyCamY(float y);
	void setDummyCamZ(float z);
	void setDummyUp(float x, float y, float z);
	void setRefPointX(float x);
	void setRefPointY(float y);
	void setRefPointZ(float z);
	void setView(viewType);
	void setPan(float x, float y, float z);

	void rotateX(float x);
	void rotateY(float y);
	void setUserCam();
	void zoom(float scale);
	void orthZoom(float scale);
	void pan(panAxis ax,float x, float y);

	void reset();
};

#endif