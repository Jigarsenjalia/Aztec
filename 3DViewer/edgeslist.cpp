#include "edgeslist.h"
#include "Vertex.h"

EdgesList::EdgesList(QWidget* parent) : QListWidget(parent) {
	edgeCount = 0;
}

EdgesList::~EdgesList() {
}

void EdgesList::setMesh(Mesh* m) {
	clear();
	edgeCount = 0;
	mesh = m;
	for (int i=0; i<mesh->getHESize(); i++)
		addItem(new EdgeItem(mesh->getHE(i)));
	edgeCount = count();
	if (count() > 0) {
		setCurrentRow(0);
		sendInfo(currentItem());
	}
}

void EdgesList::resetMesh() {
	clear();
	edgeCount = 0;
	for (int i=0; i<mesh->getHESize(); i++)
		addItem(new EdgeItem(mesh->getHE(i)));
	edgeCount = count();
	if (count() > 0) {
		setCurrentRow(0);
		sendInfo(currentItem());
	}
}

void EdgesList::setItem(QListWidgetItem* i) {
	setCurrentItem(i);
	sendInfo(i);
}

void EdgesList::sendInfo(QListWidgetItem* i) {
	emit displayInfo(((EdgeItem*)i)->edge);
}

void EdgesList::addVertex() {
	if (count() > 0) {
		HalfEdge* e = ((EdgeItem*)currentItem())->edge;
		mesh->addVertex(e);
		emit itemChanged(currentItem());
		emit vertexAdded();
		addItem(new EdgeItem(mesh->getHE(mesh->getHESize()-2)));
		addItem(new EdgeItem(mesh->getHE(mesh->getHESize()-1)));
		edgeCount += 2;
	}
}

void EdgesList::deleteVertex() {
	if (count() > 0) {
		HalfEdge* e = ((EdgeItem*)currentItem())->edge;
		mesh->deleteVertex(e);
		resetMesh();
		if (count()>0)
			emit itemChanged(currentItem());
		emit vertexDeleted();
	}
}

void EdgesList::splitQuad() {
	EdgeItem* e = new EdgeItem(mesh->getHE(edgeCount));
	addItem(e);
	addItem(new EdgeItem(mesh->getHE(edgeCount+1)));
	edgeCount += 2;
	setCurrentItem(e);
	sendInfo(e);
}

void EdgesList::addEdge() {
	EdgeItem* e = new EdgeItem(mesh->getHE(mesh->getHESize()-2));
	addItem(e);
	addItem(new EdgeItem(mesh->getHE(mesh->getHESize()-1)));
	edgeCount += 2;
	setCurrentItem(e);
	sendInfo(e);
}