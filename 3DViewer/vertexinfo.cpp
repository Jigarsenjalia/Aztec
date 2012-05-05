#include "vertexinfo.h"
#include <iostream>
#include <sstream>

VertexInfo::VertexInfo(QWidget* parent) : QTextBrowser(parent) {
}

void VertexInfo::setInfo(Vertex* v) {
	stringstream ss (stringstream::in | stringstream::out);
	ss<<"Current vertex: v";
	ss<<v->id;
	ss<<"\n";
	ss<<"Position: (";
	ss<<v->x;
	ss<<", ";
	ss<<v->y;
	ss<<", ";
	ss<<v->z;
	ss<<")";
	string str1 = ss.str();
	const char* str = str1.c_str();
	setText(str);
}