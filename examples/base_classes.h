// Base classes that will be used in the registry. Note that no knowledge of
// the registry is needed here.
// Author: Philip Salvaggio

#pragma once

#include <memory>
#include <string>

#include "cppregpattern/registry.h"

// Base class with no parameters in the constructor
class Base0 {
 public:
  Base0() = default;
  virtual ~Base0() = default;

  virtual void Print() const = 0;
};
using Base0Factory = registry::Registry<std::string, std::unique_ptr<Base0>(),
                                        registry::MissingKeyPolicy::exception>;
#define REGISTER_BASE0_SUBCLASS(Derived)                      \
  static bool _registered_##Derived = Base0Factory::Register( \
      #Derived, []() { return std::unique_ptr<Base0>(new Derived); });

// Base class with a 1-parameter constructor
class Base1 {
 public:
  explicit Base1(const Base0* printer) : printer_(printer) {}
  virtual ~Base1() = default;

  const Base0* printer() const { return printer_; }

  virtual void Print() const = 0;

 private:
  const Base0* printer_;
};
using Base1Factory =
    registry::Registry<std::string, std::unique_ptr<Base1>(const Base0*),
                       registry::MissingKeyPolicy::default_construct>;
#define REGISTER_BASE1_SUBCLASS(Derived)                      \
  static bool _registered_##Derived = Base1Factory::Register( \
      #Derived,                                               \
      [](const Base0* b) { return std::unique_ptr<Base1>(new Derived(b)); });

// Base class with a 2-parameter constructor.
class Base2 {
 public:
  Base2(const Base1* printer, int id) : printer_(printer), id_(id) {}
  virtual ~Base2() = default;

  const Base1* printer() const { return printer_; }
  int id() const { return id_; }

  virtual void Print() const = 0;

 private:
  const Base1* printer_;
  int id_;
};
using Base2Factory =
    registry::Registry<std::string, std::unique_ptr<Base2>(const Base1*, int),
                       registry::MissingKeyPolicy::optional>;
#define REGISTER_BASE2_SUBCLASS(Derived)                          \
  static bool _registered_##Derived =                             \
      Base2Factory::Register(#Derived, [](const Base1* b, int i) { \
        return std::unique_ptr<Base2>(new Derived(b, i));         \
      });
