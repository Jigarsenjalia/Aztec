#ifndef VERTEXITEM_H
#define VERTEXITEM_H

#include "Vertex.h"
#include "QListWidgetItem"
using namespace std;

class VertexItem : public QListWidgetItem{

public:
	Vertex* vertex;
	//constructors
	VertexItem(Vertex*);
	//destructor
	~VertexItem(void);
};

#endif