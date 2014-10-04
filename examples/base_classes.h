// A simple test and demonstration of the registry interface defined in
// regristy.h
//
// Author: Philip Salvaggio

#ifndef BASE_CLASSES_H
#define BASE_CLASSES_H

#include <unordered_map>

// Base class with no parameters in the constructor
class Base0 {
  public:
    Base0();
    virtual ~Base0();
    virtual void Print() const = 0;
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

#endif  // BASE_CLASSES_H
