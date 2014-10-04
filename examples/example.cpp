// File Description
// Author: Philip Salvaggio

#include "base_classes.h"
#include "registry.h"
#include <iostream>
#include <memory>

using namespace std;
using namespace registry;

// Main program, demonstrates how to construct objects with the registry.
int main() {
  std::unique_ptr<Base0> d01(Registry<Base0>::Create("Derived01"));
  std::unique_ptr<Base0> d02(Registry<Base0>::Create("Derived02"));

  d01->Print();
  d02->Print();
  
  std::unique_ptr<Base1> d11(
      Registry1<Base1, Base0*>::Create("Derived11", d01.get()));
  std::unique_ptr<Base1> d12(
      Registry1<Base1, Base0*>::Create("Derived12", d02.get()));

  d11->Print();
  d12->Print();

  std::unique_ptr<Base2> d21(
      Registry2<Base2, Base1*, int>::Create("Derived21", d11.get(), 1000));
  std::unique_ptr<Base2> d22(
      Registry2<Base2, Base1*, int>::Create("Derived22", d12.get(), 1001));

  d21->Print();
  d22->Print();
  

  return 0;
}
