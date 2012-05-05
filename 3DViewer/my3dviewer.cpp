#include "my3dviewer.h"

#define HEIGHT 427
#define WIDTH 320

My3DViewer::My3DViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	currentMode = FACE;

    QFile file("style.qss.txt");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
	setStyleSheet(styleSheet);

	objectButton = new QPushButton(QString("Object"), this);
	objectButton->hide();
	objectButton->resize(100,20);

	faceButton = new QPushButton(QString("Face"), this);
	faceButton->hide();
	faceButton->resize(100,20);
	
	edgeButton = new QPushButton(QString("Edge"), this);
	edgeButton->hide();
	edgeButton->resize(100,20);
	
	vertexButton = new QPushButton(QString("Vertex"), this);
	vertexButton->hide();
	vertexButton->resize(100,20);

	splitQuad = new QPushButton(QString("Split Face"), this);
	splitQuad->hide();
	splitQuad->resize(100,20);

	snapToFace = new QPushButton(QString("Snap to face"), this);
	snapToFace->hide();
	snapToFace->resize(100,20);
	faceOps.push_back(snapToFace);

	extrude = new QPushButton(QString("Extrude"), this);
	extrude->hide();
	extrude->resize(100,20);
	faceOps.push_back(extrude);

	insertEdge = new QPushButton(QString("Insert Edge"), this);
	insertEdge->hide();
	insertEdge->resize(100,20);
	faceOps.push_back(insertEdge);

	cpButton = new QPushButton(QString("Control Point"), this);
	cpButton->hide();
	cpButton->resize(100,20);

	faceOps.push_back(splitQuad);

	snapToEdge = new QPushButton(QString("Snape to edge"), this);
	snapToEdge->hide();
	snapToEdge->resize(100,20);
	edgeOps.push_back(snapToEdge);

	insertVertex = new QPushButton(QString("Insert Vertex"), this);
	insertVertex->hide();
	insertVertex->resize(100,20);
	edgeOps.push_back(insertVertex);

	snapToVertex = new QPushButton(QString("Snap to vertex"), this);
	snapToVertex->hide();
	snapToVertex->resize(100,20);
	vertexOps.push_back(snapToVertex);

	deleteVertex = new QPushButton(QString("Delete Vertex"), this);
	deleteVertex->hide();
	deleteVertex->resize(100,20);
	vertexOps.push_back(deleteVertex);

	incrementSharpness = new QPushButton(QString("Increment Sharpness"), this);
	incrementSharpness->hide();
	incrementSharpness->resize(120,20);
	edgeOps.push_back(incrementSharpness);

	decrementSharpness = new QPushButton(QString("Decrement Sharpness"), this);
	decrementSharpness->hide();
	decrementSharpness->resize(120,20);
	edgeOps.push_back(decrementSharpness);

	smooth = new QPushButton(QString("Smooth"), this);
	smooth->hide();
	smooth->resize(120,20);
	objectOps.push_back(smooth);

	forcePlanarity = new QPushButton(QString("Force Planarity"), this);
	forcePlanarity->hide();
	forcePlanarity->resize(120,20);
	objectOps.push_back(forcePlanarity);

	cpButton = new QPushButton(QString("Control Point"), this);
	cpButton->hide();
	cpButton->resize(100,20);

	buttons.push_back(objectButton);
	buttons.push_back(vertexButton);
	buttons.push_back(edgeButton);
	buttons.push_back(faceButton);
	buttons.push_back(splitQuad);
	buttons.push_back(deleteVertex);
	buttons.push_back(cpButton);
	buttons.push_back(insertEdge);
	buttons.push_back(incrementSharpness);
	buttons.push_back(forcePlanarity);
	buttons.push_back(decrementSharpness);
	buttons.push_back(insertVertex);
	buttons.push_back(snapToVertex);
	buttons.push_back(snapToEdge);
	buttons.push_back(snapToFace);
	buttons.push_back(extrude);
	buttons.push_back(smooth);

	for (int i=0; i<buttons.size(); i++){
		connect(buttons[i], SIGNAL(clicked()), this, SLOT(hideDisplayButtons()));
	}

	zoom = new QCursor(QPixmap("Images/zoom.png"), 0, Qt::AutoColor);
	rotate = new QCursor(QPixmap("Images/rotate.png"), 0, Qt::AutoColor);

	//color=new myColorDialog(this);
	col=new QColorDialog(this);
}

My3DViewer::~My3DViewer()
{

}

void My3DViewer::setMode(modeType newMode){
	currentMode = newMode;
}

void My3DViewer::displayModeButtons(float x, float y, modeType cMode){
	int offsetX = 12;
	int offsetY = -30;
	
	objectButton->move(x+offsetX,y+offsetY);
	objectButton->raise();
	objectButton->show();

	faceButton->move(x+faceButton->width()+15+offsetX, y + offsetY);
	faceButton->raise();
	faceButton->show();

	edgeButton->move(x+offsetX,y);
	edgeButton->raise();
	edgeButton->show();

	vertexButton->move(x+faceButton->width()+15+offsetX, y);
	vertexButton->raise();
	vertexButton->show();

	cpButton->move(x+offsetX, y+70+2*offsetY+faceButton->height());
	cpButton->raise();
	cpButton->show();

	if (cMode==FACE){
		showModeOps(x,y,faceOps);
	}

	else if (cMode==OBJECT){
		showModeOps(x,y,objectOps);
	}

	else if (cMode==EDGE){
		showModeOps(x,y,edgeOps);
	}

	else if (cMode != CP) {
		showModeOps(x,y,vertexOps);
	}
}

void My3DViewer::showModeOps(float x, float y, vector<QPushButton*> ops){
	for (int i=0; i<ops.size(); i++){
		float xPos = x + 70;// - (110 - (int)ops[i]->width());
		if (ops[i]->width() == 100){
			xPos += 10;
		}
		ops[i]->move(xPos,y+75+(i)*20);
		ops[i]->show();
	}
}

void My3DViewer::hideDisplayButtons(){
	for (int i=0; i<buttons.size(); i++){
		buttons[i]->hide();
	}
}

void My3DViewer::setAztec(MyGLWidget* m){
	aztec=m;
	connect(splitQuad, SIGNAL(clicked()), aztec, SLOT(splitQuad()));
	connect(deleteVertex, SIGNAL(clicked()), aztec, SLOT(deleteVertex()));
	connect(insertEdge, SIGNAL(clicked()), aztec, SLOT(insertEdge()));
	connect(incrementSharpness, SIGNAL(clicked()), aztec, SLOT(incSharpness()));
	connect(decrementSharpness, SIGNAL(clicked()), aztec, SLOT(decSharpness()));
	connect(forcePlanarity, SIGNAL(clicked()), aztec, SLOT(toggleForcePlanarity()));
	connect(insertVertex, SIGNAL(clicked()), aztec, SLOT(insertVertex()));
	connect(smooth, SIGNAL(clicked()), aztec, SLOT(smooth()));
	connect(extrude, SIGNAL(clicked()), aztec, SLOT(extrude2()));

	connect(faceButton, SIGNAL(clicked()), aztec, SLOT(enterFaceMode()));
	connect(objectButton, SIGNAL(clicked()), aztec, SLOT(enterObjectMode()));
	connect(vertexButton, SIGNAL(clicked()), aztec, SLOT(enterVertexMode()));
	connect(edgeButton, SIGNAL(clicked()), aztec, SLOT(enterEdgeMode()));
	connect(cpButton, SIGNAL(clicked()), aztec, SLOT(enterCPMode()));

	connect(snapToFace, SIGNAL(clicked()), aztec, SLOT(snapToFace()));
	connect(snapToEdge, SIGNAL(clicked()), aztec, SLOT(snapToEdge()));
	connect(snapToVertex, SIGNAL(clicked()), aztec, SLOT(snapToVertex()));

	//Connect mouse move events
	connect(aztec, SIGNAL(sendMouseEvent(QMouseEvent*)), this, SLOT(moveEvent(QMouseEvent*)));

		//errors
	connect(aztec, SIGNAL(error(char*)), this, SLOT(throwError(char*)));
	
}

void My3DViewer::resetCursor(){
	aztec->setCursor(Qt::ArrowCursor);
}

void My3DViewer::setZoomCursor(){
	aztec->setCursor(*zoom);
}

void My3DViewer::setRotateCursor(){
	aztec->setCursor(*rotate);
}

void My3DViewer::moveEvent(QMouseEvent* e){
	mouseMoveEvent(e);
}

void My3DViewer::mouseMoveEvent(QMouseEvent* e){
}

void My3DViewer::startLoader(){
	loader=new myFileDialog(this);

	QString openfile=QFileDialog::getOpenFileName(this, tr("Open File"),"\CIS277",tr("OBJ files (*.obj)"));

	if(openfile.compare("")!=0){
	string opens=openfile.toStdString();
	c=new char[opens.length()+1];
		for(int i=0; i<opens.length(); i++){
		c[i]=opens[i];
		cout<<c[i];
		}
		//null terminated string
		c[opens.length()]='\0';
		emit objFileName(c);
	}
}

void My3DViewer::startSave(){
	loader=new myFileDialog(this);
    QString openfile = QFileDialog::getSaveFileName(this,tr("Save Document"),QDir::currentPath(), 
		tr("OBJ files (*.obj)") );

	if(openfile.compare("")!=0){
	string opens=openfile.toStdString();
	c=new char[opens.length()+1];
		for(int i=0; i<opens.length(); i++){
		c[i]=opens[i];
		cout<<c[i];
		}
		//null terminated string
		c[opens.length()]='\0';
		emit objSaveName(c);
	}
}

void My3DViewer::fragLoader(){
	loader=new myFileDialog(this);

	QString openfile=QFileDialog::getOpenFileName(this, tr("Open File"),"\CIS277",tr("Shader files (*.frag)"));

	if(openfile.compare("")!=0){
		string opens=openfile.toStdString();
		c=new char[opens.length()+1];
			for(int i=0; i<opens.length(); i++){
			c[i]=opens[i];
			cout<<c[i];
		}
		//null terminated string
		c[opens.length()]='\0';
		emit fragFileName(c);
	}
}

void My3DViewer::vertLoader(){
	loader=new myFileDialog(this);

	QString openfile=QFileDialog::getOpenFileName(this, tr("Open File"),"\CIS277",tr("Shader files (*.vert)"));

	if(openfile.compare("")!=0){
		string opens=openfile.toStdString();
		c=new char[opens.length()+1];
			for(int i=0; i<opens.length(); i++){
			c[i]=opens[i];
			cout<<c[i];
		}
		//null terminated string
		c[opens.length()]='\0';
		emit vertFileName(c);
	}
}

void My3DViewer::mtlLoader(){
	loader=new myFileDialog(this);

	QString openfile=QFileDialog::getOpenFileName(this, tr("Open File"),"\CIS277",tr("Material files (*.mtl)"));

	if(openfile.compare("")!=0){
		string opens=openfile.toStdString();
		c=new char[opens.length()+1];
			for(int i=0; i<opens.length(); i++){
			c[i]=opens[i];
			cout<<c[i];
		}
		//null terminated string
		c[opens.length()]='\0';
		emit materialFileName(c);
	}
}

void My3DViewer::throwError(char* err) {
	error = new errorDialog(this, err);
}

void My3DViewer::openColor(){
	//color=new myColorDialog(this);
	//col->exec();
	QColor c=col->getColor(Qt::white, 0);
	emit toWidgetCol(c.red(), c.green(), c.blue());

}