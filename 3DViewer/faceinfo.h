#ifndef FACEINFO_H
#define FACEINFO_H

#include "qtextbrowser.h"
#include "Face.h"
using namespace std;

class FaceInfo : public QTextBrowser {
	Q_OBJECT;

public:
	FaceInfo(QWidget*);
	
public slots:
	void setInfo(Face*);
};
#endif