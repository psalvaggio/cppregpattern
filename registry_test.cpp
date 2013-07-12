// A simple test and demonstration of the registry interface defined in
// regristy.h
//
// Author: Philip Salvaggio

#include "registry_test.h"
#include <iostream>

using namespace std;

// Register the derived classes.
REGISTER_SUBCLASS_IMPL(Derived01, Base0)
REGISTER_SUBCLASS_IMPL(Derived02, Base0)
REGISTER_SUBCLASS_IMPL(Derived11, Base1)
REGISTER_SUBCLASS_IMPL(Derived12, Base1)
REGISTER_SUBCLASS_IMPL(Derived21, Base2)
REGISTER_SUBCLASS_IMPL(Derived22, Base2)

// Main program, demonstrates how to construct objects with the registry.
int main() {
  Base0* d01 = Base0Registry::Create("Derived01");
  Base0* d02 = Base0Registry::Create("Derived02");

  d01->Print();
  d02->Print();

  Base1* d11 = Base1Registry::Create("Derived11", d01);
  Base1* d12 = Base1Registry::Create("Derived12", d02);

  d11->Print();
  d12->Print();

  Base2* d21 = Base2Registry::Create("Derived21", d11, 1000);
  Base2* d22 = Base2Registry::Create("Derived22", d12, 1001);

  d21->Print();
  d22->Print();

  delete d01;
  delete d02;
  delete d11;
  delete d12;
  delete d21;
  delete d22;

  return 0;
}

// No parameter constructor classes.
Base0::Base0() {}
Base0::~Base0() {}

Derived01::Derived01() : Base0() {}
Derived01::~Derived01() {}
void Derived01::Print() const { cout << "Derived01" << endl; }

Derived02::Derived02() : Base0() {}
Derived02::~Derived02() {}
void Derived02::Print() const { cout << "Derived02" << endl; }


// 1-parameter constructor classes.
Base1::Base1(Base0* printer) : printer_(printer) {}
Base1::~Base1() {}

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
Base2::Base2(const Base1* printer, int id) : printer_(printer), id_(id) {}
Base2::~Base2() {}

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
