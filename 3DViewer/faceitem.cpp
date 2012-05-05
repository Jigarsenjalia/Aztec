#include "faceitem.h"

FaceItem::FaceItem(Face* f) : QListWidgetItem(){
	face = f;
	char str[10];
	itoa(f->id, str, 10);
	string str1 = str;
	string str2 = "Face " + str1;
	const char* str3 = str2.c_str();
	setText(str3);
}

FaceItem::~FaceItem() {
}