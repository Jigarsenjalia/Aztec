#ifndef VERTEXINFO_H
#define VERTEXINFO_H

#include "qtextbrowser.h"
#include "Vertex.h"
using namespace std;

class VertexInfo : public QTextBrowser {
	Q_OBJECT;

public:
	VertexInfo(QWidget*);
	
public slots:
	void setInfo(Vertex*);
};
#endif