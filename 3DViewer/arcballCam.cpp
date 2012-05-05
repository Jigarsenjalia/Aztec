#include "arcballCam.h"

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
#define DEG2RAD (M_PI/180.0)
#define RAD2DEG (180.0/M_PI)

arcballCam::arcballCam(const glm::vec3& _ref, const glm::vec3& _cam, const glm::vec3& _up,
	const float _fovy, float width, float height) {
		refPosition = _ref;
		camPosition = _cam;
		dummyCam=_cam;
		up = _up;
		dummyUp=glm::vec3(0,1,0);
		fovy = _fovy;
		aspect = width/height;
		z_near = 0.1f;
		z_far = 100.0f;
		userCam=_cam;

		startVec=glm::vec3(0,0,0);
		endVec=glm::vec3(0,0,0);

		//set adjusted width and height
		camWidth=1.0f/((width-1.0f)*0.5f);
		camHeight=1.0f/((height-1.0f)*0.5f);
		q=glm::gtc::quaternion::quat();
		qRotation=glm::mat4(1.0f);
		qPan=glm::vec3(0,0,0);
		qZoom=camPosition[2];
		qOrthZoom=1.0f;
}

arcballCam::~arcballCam(){

}

void arcballCam::click(float x, float y){
	sphereCoord(x,y, &startVec);
}

void arcballCam::sphereCoord(float x, float y, glm::vec3* vec){
	float length;

	x=x*camWidth-1.0f;
	y=1.0f-(y*camHeight);		

	length=x*x+y*y;
	float ptZ;

	if(length>1.0f){
		float n=1/sqrt(length);
		x=n*x;
		y=n*y;
		ptZ=0.0f;
	}
	else{
		ptZ=sqrt(1.0f-length);
	}

	vec->x=x;
	vec->y=y;
	vec->z=ptZ;
}

void arcballCam::drag(float x, float y, glm::vec4* quat){
	sphereCoord(x,y, &endVec);

	//compute rotation axis
	glm::vec3 axis=glm::cross(startVec, endVec);
	float angle=0.0f;

	glm::vec4 newQuat=glm::vec4(angle,axis[0],axis[1],axis[2]);
	angle=glm::dot(startVec, endVec);

	//quaternion multiplication
	float newW=newQuat[0]*quat->w-newQuat[1]*quat->x-newQuat[2]*quat->y-newQuat[3]*quat->z;
	float newX=newQuat[0]*quat->x+newQuat[1]*quat->w+newQuat[2]*quat->z-newQuat[3]*quat->y;
	float newY=newQuat[0]*quat->y-newQuat[1]*quat->z+newQuat[2]*quat->w+newQuat[3]*quat->x;
	float newZ=newQuat[0]*quat->z+newQuat[1]*quat->y-newQuat[2]*quat->x+newQuat[3]*quat->w;

	//computes quaternion from axis
	if(glm::length(axis)>FLT_EPSILON){
		//angle=glm::dot(startVec, endVec);
		//quat->x=axis[0];
		//quat->y=axis[1];
		//quat->z=axis[2];
		//quat->w=angle;
		quat->w=newW;
		quat->x=newX;
		quat->y=newY;
		quat->z=newZ;
	}
	else
		quat->w=quat->x=quat->y=quat->z=0.0f;
	q=glm::gtc::quaternion::quat(angle, axis[0], axis[1], axis[2]);
}

glm::mat3 arcballCam::setRotation(const glm::vec4* quat){

	//float xx=q->x*q->x; float yy=q->y*q->y; float zz=q->z*q->z;
	//float xy=q->x*q->y; float wz=q->w*q->z; float wy=q->w*q->y;
	//float xz=q->x*q->z; float yz=q->y*q->z; float wx=q->w*q->x;

	//qRotation=glm::mat3(1.0f-2*yy-2*zz, 2*(xy-wz), 2*(xz+wy),
	//					2*(xy+wz), 1.0f-2*xx-2*zz, 2*(yz-wx),
	//					2*(xz-wy), 2*(yz+wx), 1-2*xx-2*yy);
	qRotation=glm::gtc::quaternion::mat4_cast(q);

	glm::mat3 quatRot=glm::mat3(qRotation[0][0], qRotation[0][1], qRotation[0][2],
							qRotation[1][0], qRotation[1][1], qRotation[1][2],		
							qRotation[2][0], qRotation[2][1], qRotation[2][2]);

	return quatRot;
}

glm::mat4 arcballCam::getProjMatrix(camType c){
	glm::mat4 projectionMatrix;
	if(c==ORTHO){
		projectionMatrix=glm::ortho(-4.0f*aspect*qOrthZoom,4.0f*aspect*qOrthZoom,-4.0f*qOrthZoom,4.0f*qOrthZoom,z_near, z_far);
	}
	else{
		projectionMatrix = glm::perspective(fovy,aspect,z_near,z_far);
	}
	return projectionMatrix;
}

glm::mat4 arcballCam::getViewMatrix(camType c){
	glm::mat4 view;
	if(c==DUMMY)
		view=glm::lookAt(dummyCam, refPosition, dummyUp);
	else if(c==QUAT){
		glm::mat4 pan=glm::translate(glm::mat4(1.0f), qPan);
		//glm::mat4 transQrot=glm::transpose(qRotation);
		glm::mat4 zoom=glm::translate(glm::mat4(1.0f), glm::vec3(0,0,qZoom));
		view=glm::inverse(pan*qRotation*zoom);
	}
	return view;
}

glm::vec3 arcballCam::getCamPos(){
	return camPosition;
}

glm::vec3 arcballCam::getRef(){
	return refPosition;
}

glm::vec3 arcballCam::getUp(){
	return dummyUp;
}

glm::vec3 arcballCam::getDummyCam(){
	return dummyCam;
}

void arcballCam::setCamPosition(float x, float y, float z){
	camPosition[0]=x;
	camPosition[1]=y;
	camPosition[2]=z;
}

void arcballCam::setDummyCamX(float x){
	userCam[0]=x;
}

void arcballCam::setDummyCamY(float y){
	userCam[1]=y;
}

void arcballCam::setDummyCamZ(float z){
	userCam[2]=z;
}

void arcballCam::setDummyUp(float x, float y, float z){
	dummyUp[0]=x;
	dummyUp[1]=y;
	dummyUp[2]=z;
}

void arcballCam::setRefPointX(float x){
	refPosition[0]=x;
}

void arcballCam::setRefPointY(float y){
	refPosition[1]=y;
}

void arcballCam::setRefPointZ(float z){
	refPosition[2]=z;
}

float arcballCam::getFovy(){
	return fovy;
}

void arcballCam::rotateX(float x){
	glm::mat4 rot=glm::rotate(glm::mat4(1.0f),x, dummyUp);	
	glm::mat3 tempRot=glm::mat3(rot[0][0], rot[0][1], rot[0][2],
							rot[1][0], rot[1][1], rot[1][2],		
							rot[2][0], rot[2][1], rot[2][2]);
	//rotate camera and dummy up vector
	dummyCam=tempRot*dummyCam;
	dummyUp=tempRot*dummyUp;
}

void arcballCam::rotateY(float y){
	glm::mat4 rot=glm::rotate(glm::mat4(1.0f),y, glm::cross(dummyUp, dummyCam));
	glm::mat3 tempRot=glm::mat3(rot[0][0], rot[0][1], rot[0][2],
							rot[1][0], rot[1][1], rot[1][2],		
							rot[2][0], rot[2][1], rot[2][2]);
	//rotate camera and dummy up vector
	dummyCam=tempRot*dummyCam;
	dummyUp=tempRot*dummyUp;
}

float arcballCam::getAspect(){
	return aspect;
}

void arcballCam::setView(viewType c){
	qZoom=camPosition[2];
	dummyCam=camPosition;
	dummyUp=up;
	if(c==PERSP){
		qRotation=glm::rotate(glm::mat4(1.0f), 30.0f,glm::vec3(1,1,0));
		rotateX(30.0f);
		rotateY(-30.0f);
	}
	else if(c==FRONT){
		qRotation=glm::rotate(glm::mat4(1.0f), 0.0f,glm::vec3(1,0,0));
		//rotateX(90.0f);
	}
	else if(c==BACK){
		qRotation=glm::rotate(glm::mat4(1.0f), 180.0f,glm::vec3(1,0,0));
		rotateX(180.0f);
	}
	else if(c==LEFT){
		qRotation=glm::rotate(glm::mat4(1.0f), -90.0f,glm::vec3(0,1,0));
		rotateX(-90.0f);
	}
	else if(c==RIGHT){
		qRotation=glm::rotate(glm::mat4(1.0f), 90.0f,glm::vec3(0,1,0));
		rotateX(90.0f);
	}
	else if(c==OVERHEAD){
		qRotation=glm::rotate(glm::mat4(1.0f), -90.0f,glm::vec3(1,0,0));
		rotateY(-90.0);
	}
	else if(c==USER){
		dummyCam=userCam;
		float angle=dummyCam[0];
		qRotation=glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0,1,0));
		angle=dummyCam[1];
		glm::mat4 tempQ=glm::rotate(glm::mat4(1.0f), -1.0f*angle, glm::vec3(1,0,0));
		qZoom=dummyCam[2];
		qRotation=tempQ*qRotation;
	}
}

void arcballCam::pan(panAxis ax, float x, float y){
	if(ax==XY){
		qPan[0]-=x;
		qPan[1]-=y;
	}
	else if(ax==XZ){
		qPan[0]-=x;
		qPan[2]-=y;
	}
	else if(ax==YZ){
		qPan[1]-=x;
		qPan[2]-=y;
	}
	refPosition=qPan;
}

void arcballCam::setPan(float x, float y, float z){
	qPan[0]=x;
	qPan[1]=y;
	qPan[2]=z;
}

void arcballCam::zoom(float scale){
	qZoom+=scale;
	glm::vec3 rayPos=dummyCam + scale*(dummyCam - refPosition)/(glm::length(dummyCam - refPosition));
	dummyCam=rayPos;
}

void arcballCam::orthZoom(float scale){
	qOrthZoom+=scale;
}

void arcballCam::reset(){
	dummyCam=camPosition;
	refPosition=glm::vec3(0,0,0);
	dummyUp=up;
	qPan=glm::vec3(0,0,0);
	qZoom=camPosition[2];
	qOrthZoom=1.0f;
	glm::vec3 qPan;
	qRotation=glm::mat4(1.0f);
}