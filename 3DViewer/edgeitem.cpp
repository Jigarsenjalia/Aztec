#include "edgeitem.h"

EdgeItem::EdgeItem(HalfEdge* e) : QListWidgetItem(){
	edge = e;
	char str[10];
	itoa(e->id, str, 10);
	string str1 = str;
	string str2 = "HalfEdge " + str1;
	const char* str3 = str2.c_str();
	setText(str3);
}

EdgeItem::~EdgeItem() {
	//delete edge;
	//edge = 0;
}