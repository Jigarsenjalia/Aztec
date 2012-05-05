#ifndef STARTERCODE_H
#define STARTERCODE_H

#include <QtGui/QMainWindow>
#include <qpushbutton.h>
#include "ui_my3dviewer.h"
#include "myglwidget.h"
#include "myFileDialog.h"
#include <iostream>
#include <QFile>
#include <QCursor>
#include "myColorDialog.h"
#include <qcolordialog.h>
#include "errorDialog.h"

class My3DViewer : public QMainWindow
{
	Q_OBJECT

public:
	My3DViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~My3DViewer();
	myFileDialog * loader;
	char* c;
	errorDialog* error;

private:
	Ui::My3DViewerClass ui;
	MyGLWidget* aztec;
	modeType currentMode;
	QPushButton* objectButton;
	QPushButton* faceButton;
	QPushButton* edgeButton;
	QPushButton* vertexButton;
	QPushButton* splitQuad;
	QPushButton* cpButton;
	QPushButton* insertEdge;
	QPushButton* deleteVertex;
	QPushButton* incrementSharpness;
	QPushButton* decrementSharpness;
	QPushButton* forcePlanarity;
	QPushButton* insertVertex;
	QPushButton* snapToFace;
	QPushButton* snapToVertex;
	QPushButton* snapToEdge;
	QPushButton* extrude;
	QPushButton* smooth;
	vector<QPushButton*> objectOps;
	vector<QPushButton*> faceOps;
	vector<QPushButton*> vertexOps;
	vector<QPushButton*> edgeOps;
	vector<QPushButton*> buttons;
	void showModeOps(float, float, vector<QPushButton*>);
	void mouseMoveEvent(QMouseEvent* e);

	QCursor* zoom;
	QCursor* rotate;

	//myColorDialog* color;
	QColorDialog* col;

public slots:
	void setAztec(MyGLWidget*);
	void displayModeButtons(float, float, modeType);
	void hideDisplayButtons();
	void setMode(modeType);
	void setZoomCursor();
	void setRotateCursor();
	void resetCursor();
	void moveEvent(QMouseEvent* e);
	//errors
	void throwError(char*);

private slots:
	void startLoader();
	void fragLoader();
	void vertLoader();
	void mtlLoader();
	void startSave();

	void openColor();

signals:
	void objFileName(char*);
	void objSaveName(char*);
	void fragFileName(char*);
	void vertFileName(char*);
	void materialFileName(char*);
	void toWidgetCol(int, int, int);
};

#endif // STARTERCODE_H
