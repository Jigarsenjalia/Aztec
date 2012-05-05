#ifndef FACESLIST_H
#define FACESLIST_H

#include "faceitem.h"
#include "qlistwidget.h"
#include "Mesh.h"
using namespace std;

class FacesList : public QListWidget{
	Q_OBJECT;

public:
	Mesh* mesh;
	FacesList(QWidget*);
	~FacesList(void);

public slots:
	void setMesh(Mesh*);
	void setItem(QListWidgetItem*);
	void sendInfo(QListWidgetItem*);
	void changeR(int);
	void changeG(int);
	void changeB(int);
	void resetMesh();
	void splitQuad();
	void split();
	void pickFace(int);

signals:
	void displayInfo(Face*);
	void setR(int);
	void setG(int);
	void setB(int);
	void quadSplitted();
};
#endif