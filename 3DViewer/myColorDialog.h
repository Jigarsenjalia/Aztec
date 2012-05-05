#ifndef MYCOLORDIALOG
#define MYCOLORDIALOG

#include <QtGui/QColorDialog>
#include <qcolor.h>
#include "ui_myColorPicker.h"

using namespace std;

class myColorDialog : public QColorDialog {
	Q_OBJECT;

	public:
		myColorDialog(QWidget*);
		~myColorDialog();
		QColor currCol;
		void getSelectedCol();

	private:
		Ui::Color ui;
	
	signals:
	
};
#endif