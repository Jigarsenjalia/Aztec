#ifndef EDGEITEM_H
#define EDGEITEM_H

#include "HalfEdge.h"
#include "QListWidgetItem"
using namespace std;

class EdgeItem : public QListWidgetItem{

public:
	HalfEdge* edge;
	//constructors
	EdgeItem(HalfEdge*);
	//destructor
	~EdgeItem(void);
};

#endif