#include "my3dviewer.h"
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <qtimer.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTimer time;
	time.start();

	My3DViewer w;
	QSplashScreen *splash = new QSplashScreen;
	splash->setPixmap(QPixmap("logo.png"));
	//splash->raise();
	splash->show();
	
	QTimer::singleShot(2000, splash, SLOT(close()));

	Sleep(4000);
	w.showMaximized();

	return a.exec();
}
