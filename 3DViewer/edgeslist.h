#ifndef EDGESLIST_H
#define EDGESLIST_H

#include "edgeitem.h"
#include "qlistwidget.h"
#include "Mesh.h"
using namespace std;

class EdgesList : public QListWidget{
	Q_OBJECT;

private:
	int edgeCount;

public:
	Mesh* mesh;
	EdgesList(QWidget*);
	~EdgesList(void);

public slots:
	void setMesh(Mesh*);
	void setItem(QListWidgetItem*);
	void sendInfo(QListWidgetItem*);
	void addVertex();
	void deleteVertex();
	void resetMesh();
	void splitQuad();
	void addEdge();

signals:
	void displayInfo(HalfEdge*);
	void vertexAdded();
	void vertexDeleted();
};
#endif