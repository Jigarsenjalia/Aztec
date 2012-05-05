#ifndef FACEITEM_H
#define FACEITEM_H

#include "Face.h"
#include "QListWidgetItem"
using namespace std;

class FaceItem : public QListWidgetItem{

public:
	Face* face;
	//constructors
	FaceItem(Face*);
	//destructor
	~FaceItem(void);
};

#endif