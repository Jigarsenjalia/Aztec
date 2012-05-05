#ifndef VERTICESLIST_H
#define VERTICESLIST_H

#include "vertexitem.h"
#include "qlistwidget.h"
#include "Mesh.h"
using namespace std;

class VerticesList : public QListWidget{
	Q_OBJECT;

public:
	Mesh* mesh;
	VerticesList(QWidget*);
	~VerticesList(void);

public slots:
	void setMesh(Mesh*);
	void setItem(QListWidgetItem*);
	void sendInfo(QListWidgetItem*);
	void changeX(int);
	void changeY(int);
	void changeZ(int);
	void resetMesh();
	void addVertex();
	void addEdge();

signals:
	void displayInfo(Vertex*);
	void setX(int);
	void setY(int);
	void setZ(int);
};
#endif