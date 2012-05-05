#ifndef MYERRORDIALOG
#define MYERRORDIALOG

#include <QtGui/QDialog>
#include "ui_error.h"
#include "qtextbrowser.h"

using namespace std;

class errorDialog : public QDialog {
	Q_OBJECT;

	public:
		errorDialog(QWidget*, char*);
		~errorDialog();

	private:
		Ui::error ui;
		QTextBrowser* label;
	
	public slots:
	
};
#endif