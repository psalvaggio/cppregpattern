// An implementation of the Registry pattern, which allows derived classes of a
// common super class to be constructed by their string class name. All
// subclasses must share a common constructor in order to be constructed by this
// implementation of the registry pattern.
//
// This file provided templated Registry classes that can be used for bae
// classes with 0-4 constructor parameters. If more parameters are needed,
// follow the pattern to make a new REGISTER_SUBCLASS# macro.
//
// Example base class:
//
// class Base {
//  public:
//   explicit Base(std::ostream& os) : os_(os) {}
//   virtual void Print() = 0;
//  private:
//   std::ostream& os_;
// };
// 
// Then, derived subclasses should call the REGISTER1_SUBCLASS macro after their
// class definition. For example:
//
// class Derived1 : public Base {
//  public:
//   explicit Derived1(std::ostream& os) : Base(os) {}
//   virtual void Print() { os << "Derived1" << std::endl; }
// };
// REGISTER1_SUBCLASS(Derived1, Base, std::ostream&)
//
// Then, when an instance of one of the subclasses is desired, it can be
// constructed as:
//
// Registry<Base, std::ostream&>::Create("Derived1", std::cout);
//
// NOTE: If the base and subclasses are going to be declared and linked in a
// library (not directly with the executable), then the linker must be forced
// to include all of the subclasses, since their headers will never be
// explicitly included. On Linux this is done with:
//
// -Wl,--whole-archive -lmylib -Wl,--no-whole-archive
//
// On Max OS X, this is done with
//
// -Wl,-force_load -lmylib
//
// See CMakeLists.txt in examples/ for a CMake example.
//
// Author: Philip Salvaggio

#ifndef REGISTRY_H
#define REGISTRY_H

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace registry {

// Utility macros
#define REGISTRY_SPACE
#define REGISTRY_CONCAT(...) __VA_ARGS__

#define REGISTER_SUBCLASS(Base, Derived) \
  static bool _registered_##Derived = registry::Registry<Base>::Register( \
        #Derived, []() -> Base* { return new Derived; });

#define REGISTRY_CTOR_W_PARAMS(Base, Derived, params, param_names) \
  [](params) -> Base* { return new Derived(param_names); }

#define REGISTER_SUBCLASS1(Base, Derived, dtype1) \
  static bool _registered_##Derived = \
      registry::Registry<Base, dtype1>::Register(#Derived, \
          REGISTRY_CTOR_W_PARAMS(Base, Derived, \
              dtype1 REGISTRY_SPACE arg1, arg1));

#define REGISTER_SUBCLASS2(Base, Derived, dtype1, dtype2) \
  static bool _registered_##Derived = \
      registry::Registry<Base, dtype1, dtype2>::Register(#Derived, \
          REGISTRY_CTOR_W_PARAMS(Base, Derived, \
              REGISTRY_CONCAT(dtype1 REGISTRY_SPACE arg1, \
                              dtype2 REGISTRY_SPACE arg2), \
              REGISTRY_CONCAT(arg1, arg2)));

#define REGISTER_SUBCLASS3(Base, Derived, dtype1, dtype2, dtype3) \
  static bool _registered_##Derived = \
      registry::Registry<Base, dtype1, dtype2, dtype3>::Register(#Derived, \
          REGISTRY_CTOR_W_PARAMS(Base, Derived, \
              REGISTRY_CONCAT(dtype1 REGISTRY_SPACE arg1, \
                              dtype2 REGISTRY_SPACE arg2, \
                              dtype3 REGISTRY_SPACE arg3), \
              REGISTRY_CONCAT(arg1, arg2, arg3)));

#define REGISTER_SUBCLASS4(Base, Derived, dtype1, dtype2, dtype3, dtype4) \
  static bool _registered_##Derived = \
      registry::Registry<Base, dtype1, dtype2, dtype3, dtype4>::Register( \
          #Derived, \
          REGISTRY_CTOR_W_PARAMS(Base, Derived, \
              REGISTRY_CONCAT(dtype1 REGISTRY_SPACE arg1, \
                              dtype2 REGISTRY_SPACE arg2, \
                              dtype3 REGISTRY_SPACE arg3, \
                              dtype4 REGISTRY_SPACE arg4), \
              REGISTRY_CONCAT(arg1, arg2, arg3, arg4)));

template<typename T, typename ... Pack>
class Registry {
 public:
  using ctor_t = std::function<T*(Pack...)>;
  using map_t = std::unordered_map<std::string, ctor_t>;

  template<typename ... Args>
  static T* Create(const std::string& class_name, Args && ... pack) {
    static_assert(std::is_same<std::tuple<Args...>, std::tuple<Pack...>>::value,
                  "Registry constructor parameter mismatch");
    if (ctors().count(class_name) == 1) {
      return ctors()[class_name](std::forward<Args>(pack)...);
    }
    std::cerr << "Class " << class_name << " not registered." << std::endl;
    return nullptr;
  }

  static bool Register(const std::string& class_name, const ctor_t& ctor) {
    ctors()[class_name] = ctor;
    return true;
  }

  static bool IsRegistered(const std::string& class_name) {
    return ctors().count(class_name) == 1;
  }

  static void Unregister(const std::string& class_name) {
    ctors().erase(class_name);
  }

 private:
  static map_t& ctors() {
    static map_t ctor_map;
    return ctor_map;
  }

  Registry();
  Registry(const Registry& other);
  Registry& operator=(const Registry& other);
};

}

#endif  // REGISTRY_H
