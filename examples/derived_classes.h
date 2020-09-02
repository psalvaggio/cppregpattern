// Derived classes that will be used in the registry. Note the
// REGISTER_SUBCLASS# macros under each class definition.
// Author: Philip Salvaggio

#pragma once

#include "base_classes.h"

// Derived classes of Base0, note that they also have default constructors.
class Derived01 : public Base0 {
 public:
  Derived01() = default;
  ~Derived01() override = default;

  void Print() const override;
};
REGISTER_BASE0_SUBCLASS(Derived01)

class Derived02 : public Base0 {
 public:
  Derived02() = default;
  ~Derived02() override = default;

  void Print() const override;
};
REGISTER_BASE0_SUBCLASS(Derived02)

// Derived classes of Base1, both with matching constructors.
class Derived11 : public Base1 {
 public:
  explicit Derived11(const Base0* printer) : Base1(printer) {}
  ~Derived11() override = default;

  void Print() const override;
};
REGISTER_BASE1_SUBCLASS(Derived11)

class Derived12 : public Base1 {
 public:
  explicit Derived12(const Base0* printer) : Base1(printer) {}
  ~Derived12() override = default;

  void Print() const override;
};
REGISTER_BASE1_SUBCLASS(Derived12)

// Derived classes of Base2, both with matching constructors.
class Derived21 : public Base2 {
 public:
  Derived21(const Base1* printer, int id) : Base2(printer, id) {}
  ~Derived21() override = default;

  void Print() const override;
};
REGISTER_BASE2_SUBCLASS(Derived21)

class Derived22 : public Base2 {
 public:
  Derived22(const Base1* printer, int id) : Base2(printer, id) {}
  ~Derived22() override = default;

  void Print() const override;
};
REGISTER_BASE2_SUBCLASS(Derived22)
