// An implementation of the Registry pattern, which allows derived classes of a
// common super class to be constructed by their string class name. All
// subclasses must share a common constructor in order to be constructed by this
// implementation of the registry pattern.
//
// This file provided templated Registry classes that can be used for bae
// classes with 0-4 constructor parameters. If more parameters are needed,
// follow the pattern to make a new REGISTRY#_CREATE_METHOD macro, a new
// REGISTER_SUBCLASS# macro and a new Registry# class.
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
// Registry1<Base, std::ostream&>::Create("Derived1", std::cout);
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

#include <unordered_map>
#include <string>
#include <iostream>

namespace registry {

// Utility macros
#define REGISTRY_SPACE
#define REGISTRY_CONCAT(...) __VA_ARGS__

#define REGISTRY_CLASS_BODY(class_name) \
  public: \
   using map_t = std::unordered_map<std::string, ctor_t>; \
   \
   static bool Register(const std::string& class_name, const ctor_t& ctor) { \
     ctors()[class_name] = ctor; \
     return true; \
   } \
   \
   static void Unregister(const std::string& class_name) { \
     ctors().erase(class_name); \
   } \
  private: \
   static map_t& ctors() { \
     static map_t ctor_map; \
     return ctor_map; \
   } \
   \
   class_name(); \
   class_name(const class_name& other); \
   class_name& operator=(const class_name& other);

#define REGISTRY_CREATE_METHOD() \
  static T* Create(const std::string& class_name) { \
    if (ctors().count(class_name) == 1) { \
      return ctors()[class_name](); \
    } \
    std::cerr << "Class " << class_name << " not registered." << std::endl; \
    return nullptr; \
  }

#define REGISTRY_CREATE_METHOD_W_PARAMS(params, param_names) \
  static T* Create(const std::string& class_name, params) { \
    if (ctors().count(class_name) == 1) { \
      return ctors()[class_name](param_names); \
    } \
    std::cerr << "Class " << class_name << " not registered." << std::endl; \
    return nullptr; \
  }

// Create a registry that uses a 1-parameter constructor.
#define REGISTRY1_CREATE_METHOD(dtype1) \
  REGISTRY_CREATE_METHOD_W_PARAMS(dtype1 REGISTRY_SPACE param1, param1)

// Create a registry that uses a 2-parameter constructor.
#define REGISTRY2_CREATE_METHOD(dtype1, dtype2) \
  REGISTRY_CREATE_METHOD_W_PARAMS( \
      REGISTRY_CONCAT(dtype1 REGISTRY_SPACE param1, \
                      dtype2 REGISTRY_SPACE param2), \
      REGISTRY_CONCAT(param1, param2))

// Create a registry that uses a 3-parameter constructor.
#define REGISTRY3_CREATE_METHOD(dtype1, dtype2, dtype3) \
  REGISTRY_CREATE_METHOD_W_PARAMS( \
      REGISTRY_CONCAT(dtype1 REGISTRY_SPACE param1, \
                      dtype2 REGISTRY_SPACE param2, \
                      dtype3 REGISTRY_SPACE param3), \
      REGISTRY_CONCAT(param1, param2, param3))

// Create a registry that uses a 4-parameter constructor.
#define REGISTRY4_CREATE_METHOD(dtype1, dtype2, dtype3, dtype4) \
  REGISTRY_CREATE_METHOD_W_PARAMS( \
      REGISTRY_CONCAT(dtype1 REGISTRY_SPACE param1, \
                      dtype2 REGISTRY_SPACE param2, \
                      dtype3 REGISTRY_SPACE param3, \
                      dtype4 REGISTRY_SPACE param4), \
      REGISTRY_CONCAT(param1, param2, param3, param4))

#define REGISTER_SUBCLASS(Base, Derived) \
  static bool _registered_##Derived = registry::Registry<Base>::Register( \
        #Derived, []() -> Base* { return new Derived; });

#define REGISTRY_CTOR_W_PARAMS(Base, Derived, params, param_names) \
  [](params) -> Base* { return new Derived(param_names); }

#define REGISTER_SUBCLASS1(Base, Derived, dtype1) \
  static bool _registered_##Derived = \
      registry::Registry1<Base, dtype1>::Register(#Derived, \
          REGISTRY_CTOR_W_PARAMS(Base, Derived, \
              dtype1 REGISTRY_SPACE arg1, arg1));

#define REGISTER_SUBCLASS2(Base, Derived, dtype1, dtype2) \
  static bool _registered_##Derived = \
      registry::Registry2<Base, dtype1, dtype2>::Register(#Derived, \
          REGISTRY_CTOR_W_PARAMS(Base, Derived, \
              REGISTRY_CONCAT(dtype1 REGISTRY_SPACE arg1, \
                              dtype2 REGISTRY_SPACE arg2), \
              REGISTRY_CONCAT(arg1, arg2)));

#define REGISTER_SUBCLASS3(Base, Derived, dtype1, dtype2, dtype3) \
  static bool _registered_##Derived = \
      registry::Registry3<Base, dtype1, dtype2, dtype3>::Register(#Derived, \
          REGISTRY_CTOR_W_PARAMS(Base, Derived, \
              REGISTRY_CONCAT(dtype1 REGISTRY_SPACE arg1, \
                              dtype2 REGISTRY_SPACE arg2, \
                              dtype3 REGISTRY_SPACE arg3), \
              REGISTRY_CONCAT(arg1, arg2, arg3)));

#define REGISTER_SUBCLASS4(Base, Derived, dtype1, dtype2, dtype3, dtype4) \
  static bool _registered_##Derived = \
      registry::Registry4<Base, dtype1, dtype2, dtype3, dtype4>::Register( \
          #Derived, \
          REGISTRY_CTOR_W_PARAMS(Base, Derived, \
              REGISTRY_CONCAT(dtype1 REGISTRY_SPACE arg1, \
                              dtype2 REGISTRY_SPACE arg2, \
                              dtype3 REGISTRY_SPACE arg3, \
                              dtype4 REGISTRY_SPACE arg4), \
              REGISTRY_CONCAT(arg1, arg2, arg3, arg4)));

template<typename T>
class Registry {
 public:
  using ctor_t = std::function<T*()>;

  REGISTRY_CREATE_METHOD()
  REGISTRY_CLASS_BODY(Registry)
};

template<typename T, typename Arg1>
class Registry1 {
 public:
  using ctor_t = std::function<T*(Arg1)>;

  REGISTRY1_CREATE_METHOD(Arg1)
  REGISTRY_CLASS_BODY(Registry1)
};

template<typename T, typename Arg1, typename Arg2>
class Registry2 {
 public:
  using ctor_t = std::function<T*(Arg1,Arg2)>;

  REGISTRY2_CREATE_METHOD(Arg1,Arg2)
  REGISTRY_CLASS_BODY(Registry2)
};

template<typename T, typename Arg1, typename Arg2, typename Arg3>
class Registry3 {
 public:
  using ctor_t = std::function<T*(Arg1,Arg2,Arg3)>;

  REGISTRY3_CREATE_METHOD(Arg1,Arg2,Arg3)
  REGISTRY_CLASS_BODY(Registry3)
};

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
class Registry4 {
 public:
  using ctor_t = std::function<T*(Arg1,Arg2,Arg3,Arg4)>;

  REGISTRY4_CREATE_METHOD(Arg1,Arg2,Arg3,Arg4)
  REGISTRY_CLASS_BODY(Registry4)
};

}

#endif  // REGISTRY_H
