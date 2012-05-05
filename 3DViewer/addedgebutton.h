#ifndef ADDEDGEBUTTON_H
#define ADDEDGEBUTTON_H

#include "qpushbutton.h"
using namespace std;

class AddEdgeButton : public QPushButton {
	Q_OBJECT;

private:
	int fa;
	int fb;
	int hea;
	int heb;

public:
	AddEdgeButton(QWidget*);

public slots:
	void setfa(int);
	void setfb(int);
	void sethea(int);
	void setheb(int);
	void addEdge(void);

signals:
	void addEdgeInfo(int,int,int,int);
};

#endif