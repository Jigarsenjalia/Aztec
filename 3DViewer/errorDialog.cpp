#include "errorDialog.h"
#include "qfont.h"

errorDialog::errorDialog(QWidget *parent, char* err)	: QDialog(parent)
{	
	ui.setupUi(this);
	label = new QTextBrowser(this);
	label->setFont(QFont("", 16));
	label->setText(err);
	label->resize(this->width(), this->height());
	setModal(true);
	this->show();
}


errorDialog::~errorDialog()
{
	delete label;
}