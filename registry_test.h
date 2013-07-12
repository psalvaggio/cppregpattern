// A simple test and demonstration of the registry interface defined in
// regristy.h
//
// Author: Philip Salvaggio

#ifndef REGISTRY_TEST_H
#define REGISTRY_TEST_H

#include "registry.h"

// Base class with no parameters in the constructor
class Base0 {
  public:
    Base0();
    virtual ~Base0();
    virtual void Print() const = 0;
};

CREATE_REGISTRY(Base0)

// Derived classes of Base0, note that they also have default constructors.
class Derived01 : public Base0 {
  public:
    Derived01();
    virtual ~Derived01();
    virtual void Print() const;
    REGISTER_SUBCLASS(Derived01, Base0)
};

class Derived02 : public Base0 {
  public:
    Derived02();
    virtual ~Derived02();
    virtual void Print() const;
    REGISTER_SUBCLASS(Derived02, Base0)
};


// Base class with a 1-parameter constructor
class Base1 {
  public:
    explicit Base1(Base0* printer);
    virtual ~Base1();
    Base0* printer() const { return printer_; }
    virtual void Print() const = 0;

  private:
    Base0* printer_;
};

CREATE_REGISTRY_1(Base1, Base0*);

// Derived classes of Base1, both with matching constructors.
class Derived11 : public Base1 {
  public:
    explicit Derived11(Base0* printer);
    virtual ~Derived11();
    virtual void Print() const;
    REGISTER_SUBCLASS(Derived11, Base1)
};

class Derived12 : public Base1 {
  public:
    explicit Derived12(Base0* printer);
    virtual ~Derived12();
    virtual void Print() const;
    REGISTER_SUBCLASS(Derived12, Base1)
};


// Base class with a 2-parameter constructor.
class Base2 {
 public:
  Base2(const Base1* printer, int id);
  virtual ~Base2();
  const Base1* printer() const { return printer_; }
  int id() const { return id_; }
  virtual void Print() const = 0;

 private:
  const Base1* printer_;
  int id_;
};

CREATE_REGISTRY_2(Base2, const Base1*, int)

// Derived classes of Base2, both with matching constructors.
class Derived21 : public Base2 {
  public:
    Derived21(const Base1* printer, int id);
    virtual ~Derived21();
    virtual void Print() const;
    REGISTER_SUBCLASS(Derived21, Base2)
};

class Derived22 : public Base2 {
  public:
    Derived22(const Base1* printer, int id);
    virtual ~Derived22();
    virtual void Print() const;
    REGISTER_SUBCLASS(Derived22, Base2)
};

#endif  // REGISTRY_TEST_H
