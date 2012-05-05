#ifndef MYDIALOG
#define MYDIALOG

#include <QtGui/QFileDialog>
#include "ui_objLoader.h"

using namespace std;

class myFileDialog : public QFileDialog {
	Q_OBJECT;

	public:
		myFileDialog(QWidget*);
		~myFileDialog();

	private:
		Ui::objLoader ui;
	
	public slots:
	
};
#endif