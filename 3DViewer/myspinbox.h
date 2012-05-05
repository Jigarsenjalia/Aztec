#ifndef MYSPINBOX_H
#define MYSPINBOX_H

#include <QSpinBox>

class MySpinBox : public QSpinBox {
	Q_OBJECT

private:

public:
	MySpinBox(QWidget*);
	~MySpinBox(void);

public slots:
	void reset0();
	void reset2();

};
#endif