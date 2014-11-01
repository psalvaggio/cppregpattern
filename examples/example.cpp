// Example usage of the registry.
// Author: Philip Salvaggio

#include "base_classes.h"
#include "registry.h"

#include <iostream>
#include <memory>

using namespace std;
using namespace registry;

int main() {
  cout << "Expected Output:" << endl
       << "Derived01" << endl
       << "Derived02" << endl
       << "Derived11: Derived01" << endl
       << "Derived12: Derived02" << endl
       << "Derived21 (1000): Derived11: Derived01" << endl
       << "Derived22 (1001): Derived12: Derived02" << endl << endl
       << "Output: " << endl;

  // No-parameter case.
  unique_ptr<Base0> d01(Registry<Base0>::Create("Derived01"));
  unique_ptr<Base0> d02(Registry<Base0>::Create("Derived02"));

  // 1-parameter case.
  unique_ptr<Base1> d11(
      Registry<Base1, Base0*>::Create("Derived11", d01.get()));
  unique_ptr<Base1> d12(
      Registry<Base1, Base0*>::Create("Derived12", d02.get()));

  // 2-parameter case.
  unique_ptr<Base2> d21(
      Registry<Base2, Base1*, int>::Create("Derived21", d11.get(), 1000));
  unique_ptr<Base2> d22(
      Registry<Base2, Base1*, int>::Create("Foo", d12.get(), 1001));

  d01->Print();
  d02->Print();
  d11->Print();
  d12->Print();
  d21->Print();
  d22->Print();

  return 0;
}
