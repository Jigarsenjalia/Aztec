#include "verticeslist.h"

VerticesList::VerticesList(QWidget* parent) : QListWidget(parent) {
}

VerticesList::~VerticesList() {
}

void VerticesList::setMesh(Mesh* m) {
	clear();
	mesh = m;
	for (int i=0; i<mesh->getVSize(); i++)
		addItem(new VertexItem(mesh->getV(i)));
	if (count() > 0) {
		setCurrentRow(0);
		sendInfo(currentItem());
	}
}

void VerticesList::resetMesh() {
	clear();
	for (int i=0; i<mesh->getVSize(); i++)
		addItem(new VertexItem(mesh->getV(i)));
	if (count() > 0) {
		setCurrentRow(0);
		sendInfo(currentItem());
	}
}


void VerticesList::addVertex() {
	VertexItem* v = new VertexItem(mesh->getV(mesh->getVSize()-1));
	addItem(v);
	setCurrentItem(v);
	sendInfo(v);
}

void VerticesList::addEdge() {
	VertexItem* v = new VertexItem(mesh->getV(mesh->getVSize()-1));
	addItem(v);
	setCurrentItem(v);
	sendInfo(v);
}


void VerticesList::setItem(QListWidgetItem* i) {
	setCurrentItem(i);
	sendInfo(i);
}

void VerticesList::sendInfo(QListWidgetItem* i) {
	Vertex* v = ((VertexItem*)i)->vertex;
	emit displayInfo(v);
	int x = (int)(v->x * 20.0f);
	int y = (int)(v->y * 20.0f);
	int z = (int)(v->z * 20.0f);
	emit setX(x);
	emit setY(y);
	emit setZ(z);
}

void VerticesList::changeX(int x) {
	if (count() > 0) {
		Vertex* v = ((VertexItem*)currentItem())->vertex;
		v->x = ((float)x)/20.0f;
		emit itemChanged(currentItem());
	}
}

void VerticesList::changeY(int y) {
	if (count() > 0) {
		Vertex* v = ((VertexItem*)currentItem())->vertex;
		v->y = ((float)y)/20.0f;
		emit itemChanged(currentItem());
	}
}

void VerticesList::changeZ(int z) {
	if (count() > 0) {
		Vertex* v = ((VertexItem*)currentItem())->vertex;
		v->z = ((float)z)/20.0f;
		emit itemChanged(currentItem());
	}
}