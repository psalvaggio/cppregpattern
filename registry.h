// A simple implementation of the Registry pattern, which allows derived
// classes of a common super class to be constructed by their string class name.
// All subclasses must share a common constructor in order to be constructed by
// this implementation of the registry pattern.
//
// A new Registry class is created for each base class to which the registry
// pattern is to be applied. This is accomplished with the CREATE_REGISTRY[_N]
// macros, where N is the number of parameters in the shared constructor.
// Macros to create registries that use up to 4-parameter constructors have
// been provided. If you need more parameters, just follow the patter to create
// a new macro.
//
// Example base class:
//
// class Base {
//  public:
//   explicit Base(std::ostream& os);
//   virtual void Print() = 0;
//  private:
//   std::ostream& os_;
// };
// CREATE_REGISTRY_1(Base, std::ostream&)
// 
// Then, derived subclasses should call the REGISTER_SUBCLASS macro in their
// class definition. For example:
//
// class Derived1 : public Base {
//  public:
//   explicit Derived1(std::ostream& os);
//   virtual void Print() { os << "Derived1" << std::endl; }
//
//  REGISTER_SUBCLASS(Derived1, Base)
// };
//
// Then in the .cpp file, the REGISTER_SUBCLASS_IMPL macro should be called.
// For example:
//
// REGISTER_SUBCLASS_IMPL(Derived1, Base)
//
// Then, when an instance of one of the subclasses is desired, it can be
// constructed as:
//
// BaseRegistry::Create("Derived1", std::cout);
//
// Author: Philip Salvaggio

#ifndef REGISTRY_H
#define REGISTRY_H

#include <map>
#include <string>
#include <iostream>

// Call this macro as the last line in the class definition of the subclass
// that need registered.
#define REGISTER_SUBCLASS(class_name, superclass) \
 private: \
  static const superclass##CreatorImpl<class_name> creator_;

// Call this at the top of the implementation file for the class that needs
// registered.
#define REGISTER_SUBCLASS_IMPL(class_name, superclass) \
  const superclass##CreatorImpl<class_name> class_name::creator_(#class_name);

// Utility macros
#define REGISTRY_SPACE
#define REGISTRY_CONCAT(...) __VA_ARGS__

// Create a registry that uses the default constructor.
#define CREATE_REGISTRY_0(superclass) \
  CREATE_REGISTRY_CREATORS(superclass,,) \
  CREATE_REGISTRY_CLASS_HEADER(superclass) \
  CREATE_REGISTRY_CREATE_DEF_NO_PARAMS(superclass) \
  CREATE_REGISTRY_CLASS_FOOTER(superclass) \
  CREATE_REGISTRY_IMPL(superclass)

// Alias for CREATE_REGISTRY_0
#define CREATE_REGISTRY(superclass) CREATE_REGISTRY_0(superclass)

// Create a registry that uses a 1-parameter constructor.
#define CREATE_REGISTRY_1(superclass, dtype1) \
  CREATE_REGISTRY_W_PARAMS(superclass, \
                           dtype1 REGISTRY_SPACE param1, \
                           param1)

// Create a registry that uses a 2-parameter constructor.
#define CREATE_REGISTRY_2(superclass, dtype1, dtype2) \
  CREATE_REGISTRY_W_PARAMS(superclass, \
      REGISTRY_CONCAT(dtype1 REGISTRY_SPACE param1, \
                      dtype2 REGISTRY_SPACE param2), \
      REGISTRY_CONCAT(param1, param2))

// Create a registry that uses a 3-parameter constructor.
#define CREATE_REGISTRY_3(superclass, dtype1, dtype2, dtype3) \
  CREATE_REGISTRY_W_PARAMS(superclass, \
      REGISTRY_CONCAT(dtype1 REGISTRY_SPACE param1, \
                      dtype2 REGISTRY_SPACE param2, \
                      dtype3 REGISTRY_SPACE param3), \
      REGISTRY_CONCAT(param1, param2, param3))

// Create a registry that uses a 4-parameter constructor.
#define CREATE_REGISTRY_4(superclass, dtype1, dtype2, dtype3, dtype4) \
  CREATE_REGISTRY_W_PARAMS(superclass, \
      REGISTRY_CONCAT(dtype1 REGISTRY_SPACE param1, \
                      dtype2 REGISTRY_SPACE param2, \
                      dtype3 REGISTRY_SPACE param3, \
                      dtype4 REGISTRY_SPACE param4), \
      REGISTRY_CONCAT(param1, param2, param3, param4))


// Helper macro for creating registries with parameters in the constructor
#define CREATE_REGISTRY_W_PARAMS(superclass, ctor_params, param_names) \
  CREATE_REGISTRY_CREATORS(superclass, \
                           REGISTRY_CONCAT(ctor_params), \
                           REGISTRY_CONCAT(param_names)) \
  CREATE_REGISTRY_CLASS_HEADER(superclass) \
  CREATE_REGISTRY_CREATE_DEF(superclass, \
                             REGISTRY_CONCAT(ctor_params), \
                             REGISTRY_CONCAT(param_names)) \
  CREATE_REGISTRY_CLASS_FOOTER(superclass) \
  CREATE_REGISTRY_IMPL(superclass)


// Create the abstract base Creator class and the CreatorImpl concrete subclass.
// The Creator class specifies the method for creating the base class. The
// CreatorImpl class is templated, and the REGISTER_SUBCLASS macros give
// subclasses static CreatorImpl members. For instance, a class Derived that
// inherits from Base will have a static const BaseCreatorImpl<Derived>
// instance. Thus, the create() method will return an instance of Derived.
#define CREATE_REGISTRY_CREATORS(superclass, ctor_params, param_names) \
class superclass##Creator { \
 public: \
  superclass##Creator(const std::string& class_name); \
  virtual ~superclass##Creator() {} \
  virtual superclass* create(ctor_params) = 0; \
}; \
\
template <class T> \
class superclass##CreatorImpl : public superclass##Creator { \
 public: \
  superclass##CreatorImpl(const std::string& class_name) \
      : superclass##Creator(class_name) {} \
  virtual ~superclass##CreatorImpl() {} \
\
  virtual superclass* create(ctor_params) { return new T(param_names); } \
}; \

// Create most of the definition of teh Registry class, up until the Create()
// method, which varies based on whether there are constructor parameters.
// Essentially, we have a static member, which is a map which relates the
// string class name to the instance of the static Creator instance in each of
// the subclasses that were created by the REGISTER_SUBCLASS macro. The
// registry class offers methods to register and unregister classes. Note that
// the Creator member were staticly allocated in the subclasses, so we don't
// need to worry about freeing them.
#define CREATE_REGISTRY_CLASS_HEADER(superclass) \
class superclass##Registry { \
 public: \
  typedef std::map<std::string, superclass##Creator* > map_t; \
\
  static void Register(const std::string& class_name, \
                       superclass##Creator* ctor) { \
    ctors()[class_name] = ctor; \
  } \
\
  static void Unregister(const std::string& class_name, \
                         superclass##Creator* ctor) { \
    ctors().erase(class_name); \
  } 

// Define the version of the Create() method that has constructo parameters. It
// gets the Creator instance from the map and passes on the parameters to its
// create() method.
#define CREATE_REGISTRY_CREATE_DEF(superclass, ctor_params, param_names) \
  static superclass* Create(const std::string& class_name, ctor_params) { \
    if (ctors().count(class_name) == 1) { \
      return ctors()[class_name]->create(param_names); \
    } \
    std::cerr << "Class " << class_name << " not registered." << std::endl; \
    return NULL; \
  } 

// Case for no constuctor parameters. Unfortunately, variadic macros couldn't
// work here, since we had two additional parameters and variadic macros don't
// allow you to address the variable parameters individually.
#define CREATE_REGISTRY_CREATE_DEF_NO_PARAMS(superclass) \
  static superclass* Create(const std::string& class_name) { \
    if (ctors().count(class_name) == 1) { \
      return ctors()[class_name]->create(); \
    } \
    std::cerr << "Class " << class_name << " not registered." << std::endl; \
    return NULL; \
  } 

// The rest of the Registry class definition. Provides the method to get access
// to the Creator map and prevents construction/copying of the Registry class,
// since all of the methods are static.
#define CREATE_REGISTRY_CLASS_FOOTER(superclass) \
 private: \
  static map_t& ctors(); \
\
  superclass##Registry(); \
  superclass##Registry(const superclass##Registry& other); \
  superclass##Registry& operator=(const superclass##Registry& other); \
}; \

// Inline implementation for a few of the class definitions above. The
// constructor for the Creator class registers the subclass in the registry.
// Since the creator instances are const static members of the subclasses, this
// constructor is called during static initialization. The ctors() static
// method in the Registry class gives access to the Creator map. This is
// implemented as a method to ensure that the static table is instantiated
// before any of the Creator objects attempts to register their classes. Note
// that in the Register() method, ctors() is called before the map insertion,
// so the table will always be initialized first.
#define CREATE_REGISTRY_IMPL(superclass) \
inline superclass##Creator::superclass##Creator( \
    const std::string& class_name) { \
  superclass##Registry::Register(class_name, this); \
} \
\
superclass##Registry::map_t& superclass##Registry::ctors() { \
  static map_t table; \
  return table; \
}

#endif  // REGISTRY_H
