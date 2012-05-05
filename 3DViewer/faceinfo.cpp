#include "faceinfo.h"
#include <iostream>
#include <sstream>

FaceInfo::FaceInfo(QWidget* parent) : QTextBrowser(parent) {
}

void FaceInfo::setInfo(Face* f) {
	stringstream ss (stringstream::in | stringstream::out);
	ss<<"Current face: f";
	ss<<f->id;
	ss<<"\n";
	ss<<"HalfEdge: e";
	ss<<f->he->id;
	ss<<"\nColor: (";
	ss<<f->color[0];
	ss<<", ";
	ss<<f->color[1];
	ss<<", ";
	ss<<f->color[2];
	ss<<")";
	string str1 = ss.str();
	const char* str = str1.c_str();
	setText(str);
}