#ifndef EDGEINFO_H
#define EDGEINFO_H

#include "qtextbrowser.h"
#include "HalfEdge.h"
using namespace std;

class EdgeInfo : public QTextBrowser {
	Q_OBJECT;

public:
	EdgeInfo(QWidget*);
	
public slots:
	void setInfo(HalfEdge*);
};
#endif