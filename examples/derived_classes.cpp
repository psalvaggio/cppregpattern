// Implementation file for derived_classes.h. No registry knowledge here.
// Author: Philip Salvaggio

#include "derived_classes.h"

#include <iostream>

// No parameter constructor classes.
void Derived01::Print() const { std::cout << "Derived01" << std::endl; }
void Derived02::Print() const { std::cout << "Derived02" << std::endl; }

// 1-parameter constructor classes.
void Derived11::Print() const {
  std::cout << "Derived11: ";
  printer()->Print();
}
void Derived12::Print() const {
  std::cout << "Derived12: ";
  printer()->Print();
}

// 2-parameter constructor classes.
void Derived21::Print() const {
  std::cout << "Derived21 (" << id() << "): ";
  printer()->Print();
}
void Derived22::Print() const {
  std::cout << "Derived22 (" << id() << "): ";
  printer()->Print();
}
