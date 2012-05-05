#ifndef MYDOUBLESPINBOX_H
#define MYDOUBLESPINBOX_H

#include <QDoubleSpinBox>

class MyDoubleSpinBox : public QDoubleSpinBox {
	Q_OBJECT

private:

public:
	MyDoubleSpinBox(QWidget*);
	~MyDoubleSpinBox(void);

public slots:
	void reset0();

};
#endif