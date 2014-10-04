// Implementation file for derived_classes.h. No registry knowledge here.
// Author: Philip Salvaggio

#include "derived_classes.h"

#include <iostream>

using namespace std;

// No parameter constructor classes.
Derived01::Derived01() : Base0() {}
Derived01::~Derived01() {}
void Derived01::Print() const { cout << "Derived01" << endl; }

Derived02::Derived02() : Base0() {}
Derived02::~Derived02() {}
void Derived02::Print() const { cout << "Derived02" << endl; }

// 1-parameter constructor classes.
Derived11::Derived11(Base0* printer) : Base1(printer) {}
Derived11::~Derived11() {}
void Derived11::Print() const {
  cout << "Derived11: ";
  printer()->Print();
}

Derived12::Derived12(Base0* printer) : Base1(printer) {}
Derived12::~Derived12() {}
void Derived12::Print() const {
  cout << "Derived12: ";
  printer()->Print();
}

// 2-parameter constructor classes.
Derived21::Derived21(const Base1* printer, int id) : Base2(printer, id) {}
Derived21::~Derived21() {}
void Derived21::Print() const {
  cout << "Derived21 (" << id() << "): ";
  printer()->Print();
}

Derived22::Derived22(const Base1* printer, int id) : Base2(printer, id) {}
Derived22::~Derived22() {}
void Derived22::Print() const {
  cout << "Derived22 (" << id() << "): ";
  printer()->Print();
}
