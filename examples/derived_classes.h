// Derived classes that will be used in the registry. Note the
// REGISTER_SUBCLASS# macros under each class definition.
// Author: Philip Salvaggio

#ifndef DERIVED_CLASSES_H
#define DERIVED_CLASSES_H

#include "base_classes.h"
#include "registry.h"

// Derived classes of Base0, note that they also have default constructors.
class Derived01 : public Base0 {
 public:
  Derived01();
  virtual ~Derived01();

  virtual void Print() const;
};
REGISTER_SUBCLASS(Base0, Derived01)

class Derived02 : public Base0 {
 public:
  Derived02();
  virtual ~Derived02();

  virtual void Print() const;
};
REGISTER_SUBCLASS(Base0, Derived02)

// Derived classes of Base1, both with matching constructors.
class Derived11 : public Base1 {
 public:
  explicit Derived11(Base0* printer);
  virtual ~Derived11();

  virtual void Print() const;
};
REGISTER_SUBCLASS1(Base1, Derived11, Base0*)

class Derived12 : public Base1 {
 public:
  explicit Derived12(Base0* printer);
  virtual ~Derived12();

  virtual void Print() const;
};
REGISTER_SUBCLASS1(Base1, Derived12, Base0*)

// Derived classes of Base2, both with matching constructors.
class Derived21 : public Base2 {
 public:
  Derived21(const Base1* printer, int id);
  virtual ~Derived21();

  virtual void Print() const;
};
REGISTER_SUBCLASS2(Base2, Derived21, Base1*, int)

class Derived22 : public Base2 {
 public:
  Derived22(const Base1* printer, int id);
  virtual ~Derived22();

  virtual void Print() const;
};
REGISTER_SUBCLASS2(Base2, Derived22, Base1*, int)

#endif  // DERIVED_CLASSES_H
