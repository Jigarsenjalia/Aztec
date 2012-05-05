#include "addedgebutton.h"

AddEdgeButton::AddEdgeButton(QWidget* parent) : QPushButton(parent) {
	fa = 0;
	fb = 0;
	hea = 0;
	heb = 0;
}

void AddEdgeButton::setfa(int i) {
	fa = i;
}

void AddEdgeButton::setfb(int i) {
	fb = i;
}

void AddEdgeButton::sethea(int i) {
	hea = i;
}

void AddEdgeButton::setheb(int i) {
	heb = i;
}

void AddEdgeButton::addEdge() {
	emit addEdgeInfo(fa, fb, hea, heb);
}