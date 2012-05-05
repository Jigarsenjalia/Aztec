#include "MyDoubleSpinBox.h"

MyDoubleSpinBox::MyDoubleSpinBox(QWidget* parent) : QDoubleSpinBox(parent) {
}

MyDoubleSpinBox::~MyDoubleSpinBox() {
}

void MyDoubleSpinBox::reset0() {
	setValue(0);
}
