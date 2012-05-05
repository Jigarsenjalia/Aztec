#include "vertexitem.h"

VertexItem::VertexItem(Vertex* v) : QListWidgetItem(){
	vertex = v;
	char str[10];
	itoa(v->id, str, 10);
	string str1 = str;
	string str2 = "Vertex " + str1;
	const char* str3 = str2.c_str();
	setText(str3);
}

VertexItem::~VertexItem() {
	//delete vertex;
	//vertex = 0;
}