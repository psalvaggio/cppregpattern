// Example usage of the registry.
// Author: Philip Salvaggio

#include <iostream>
#include <memory>

#include "base_classes.h"

int main() {
  std::cout << "Expected Output:\n"
               "Derived01\n"
               "Derived02\n"
               "Derived11: Derived01\n"
               "Derived12: Derived02\n"
               "Derived21 (1000): Derived11: Derived01\n"
               "Derived22 (1001): Derived12: Derived02\n\n"
               "Output: \n";

  // No-parameter case.
  auto d01 = Base0Factory::Dispatch("Derived01");
  auto d02 = Base0Factory::Dispatch("Derived02");

  // 1-parameter case.
  auto d11 = Base1Factory::Dispatch("Derived11", d01.get());
  auto d12 = Base1Factory::Dispatch("Derived12", d02.get());

  // 2-parameter case.
  auto d21 = Base2Factory::Dispatch("Derived21", d11.get(), 1000);
  auto d22 = Base2Factory::Dispatch("Derived22", d12.get(), 1001);

  d01->Print();
  d02->Print();
  d11->Print();
  d12->Print();
  (*d21)->Print();
  (*d22)->Print();

  try {
    auto d = Base0Factory::Dispatch("Foo");
    std::cerr << "Base0Factory::Dispatch(\"Foo\") did not throw" << std::endl;
    return 1;
  } catch (const std::exception& e) {
  }

  if (auto d = Base1Factory::Dispatch("Foo", nullptr); d != nullptr) {
    std::cerr
        << "Base1Factory::Dispatch(\"Foo\", nullptr) did not return nullptr"
        << std::endl;
    return 1;
  }

  if (auto d = Base2Factory::Dispatch("Foo", nullptr, 0); d) {
    std::cerr << "Base1Factory::Dispatch(\"Foo\", nullptr, 0) did not return "
                 "std::nullopt"
              << std::endl;
    return 1;
  }

  return 0;
}
