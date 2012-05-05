#include "MySpinBox.h"

MySpinBox::MySpinBox(QWidget* parent) : QSpinBox(parent) {
}

MySpinBox::~MySpinBox() {
}

void MySpinBox::reset0() {
	setValue(0);
}

void MySpinBox::reset2() {
	setValue(2);
}