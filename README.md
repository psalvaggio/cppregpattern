# C++ Registry Pattern

An implementation of the Registry pattern, which allows derived classes of a common super class to be constructed by their string class name. All subclasses must share a common constructor in order to be constructed by this implementation of the registry pattern.

This library defines a class template

```c++
Registry<Base, Args...>
```

where Base is the base class of the inheritance tree and Args are the parameters of the common constructor. It is recommended to use a template alias for code readability. For instance, if the shared constructor takes an int and a string const reference,

```c++
using BaseFactory = Registry<Base, int, const std::string&>;
```

In order to register a subclass, a number of macros have been defined in registry.h. If the string class name is to be used for constructing the class, the use,

```c++
class Derived : public Base {
 public:
  Derived(int id, const std::string& name);
...
};
REGISTER_SUBCLASS(Base, Derived, int, const std::string&) 
```

If a custom identifier is going to be used to construct the subclass, instead use

```c++
REGISTER_SUBCLASS_W_IDENTIFIER(Base, Derived, Identifier, int, const std::string&)
```

To then create objects using the Registry class, simply call the Create function with the class name/identifier and the constructor parameters.

```c++
using BaseFactory = Registry<Base, int, const std::string&>;
std::unique_ptr<Base> obj(BaseFactory::Create("Identifier", 0, "foo"));
```

*IMPORTANT*: This scheme works as is when all of the classes to be constructed are linked directly to the executable. However, if they are in an external library, the linker must be forced to load all of the symbols in that library. On Linux, this is done with the flags

```
-Wl,--whole-archive -lmylib -Wl,--no-whole-archive
```

on Max OS X, this is done with the flags

```
-Wl,-force_load -lmylib
```

See the examples directory for an example with CMake.
