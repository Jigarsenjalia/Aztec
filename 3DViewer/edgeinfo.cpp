#include "edgeinfo.h"
#include <iostream>
#include <sstream>

EdgeInfo::EdgeInfo(QWidget* parent) : QTextBrowser(parent) {
}

void EdgeInfo::setInfo(HalfEdge* e) {
	stringstream ss (stringstream::in | stringstream::out);
	ss<<"Current halfedge: e";
	ss<<e->id;
	ss<<"\nVertex: v";
	ss<<e->v->id;
	ss<<" (";
	ss<<e->v->x;
	ss<<", ";
	ss<<e->v->y;
	ss<<", ";
	ss<<e->v->z;
	ss<<")\nNext: e";
	ss<<e->next->id;
	if (e->sym) {
		ss<<"\nSym: e";
		ss<<e->sym->id;
	}
	ss<<"\nFace: f";
	ss<<e->f->id;
	string str1 = ss.str();
	const char* str = str1.c_str();
	setText(str);
}