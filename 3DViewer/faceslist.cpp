#include "faceslist.h"

FacesList::FacesList(QWidget* parent) : QListWidget(parent) {
}

FacesList::~FacesList() {
}

void FacesList::setMesh(Mesh* m) {
	clear();
	mesh = m;
	for (int i=0; i<mesh->getFSize(); i++)
		addItem(new FaceItem(mesh->getF(i)));
	if (count() > 0) {
		setCurrentRow(0);
		sendInfo(currentItem());
	}
}

void FacesList::resetMesh() {
	clear();
	for (int i=0; i<mesh->getFSize(); i++)
		addItem(new FaceItem(mesh->getF(i)));
	if (count() > 0) {
		setCurrentRow(0);
		sendInfo(currentItem());
	}
}

void FacesList::setItem(QListWidgetItem* i) {
	setCurrentItem(i);
	sendInfo(i);
}

void FacesList::sendInfo(QListWidgetItem* i) {
	Face* f = ((FaceItem*)i)->face;
	emit displayInfo(f);
	int r = (int)(f->color[0] * 255.0f);
	int g = (int)(f->color[1] * 255.0f);
	int b = (int)(f->color[2] * 255.0f);
	emit setR(r);
	emit setG(g);
	emit setB(b);
}

void FacesList::changeR(int r) {
	if (count() > 0) {
		Face* f = ((FaceItem*)currentItem())->face;
		f->color[0] = ((float)r)/255.0f;
		emit itemChanged(currentItem());
	}
}

void FacesList::changeG(int g) {
	if (count() > 0) {
		Face* f = ((FaceItem*)currentItem())->face;
		f->color[1] = ((float)g)/255.0f;
		emit itemChanged(currentItem());
	}
}

void FacesList::changeB(int b) {
	if (count() > 0) {
		Face* f = ((FaceItem*)currentItem())->face;
		f->color[2] = ((float)b)/255.0f;
		emit itemChanged(currentItem());
	}
}

void FacesList::splitQuad() {
	if (count() > 0) {
		Face* f = ((FaceItem*)currentItem())->face;
		if (mesh->splitQuad(f)) {
			addItem(new FaceItem(mesh->getF(mesh->getFSize()-1)));
			emit itemChanged(currentItem());
			emit quadSplitted();
		}
	}
}

void FacesList::split() {
	if (count() > 0) {
		Face* f = ((FaceItem*)currentItem())->face;
		addItem(new FaceItem(mesh->getF(mesh->getFSize()-1)));
			emit itemChanged(currentItem());
			emit quadSplitted();
	}
}

void FacesList::pickFace(int i) {
	setCurrentRow(i);
	sendInfo(currentItem());
}