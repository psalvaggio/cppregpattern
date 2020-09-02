# C++ Registry Pattern

A self-registering map of functions, allowing for dynamic dispatching based
on some identifier. Example usages may be constructing a subclass or using
an appropriate I/O function based on an enum value or a string key. All
functions in the map need to have the same signature in this implementation.
Multiple threads can read from the map using Dispatch() at the same time,
but do not call Register() and Dispatch() from different threads as the
same time.

Since the Registry template instantiation can be quite verbose and
registration is a bit of boilerplate, it is recommended that users create
a type alias and a macro for registration. For example:
```c++
using BaseRegistry = Registry<std::string,
                              std::unique_ptr<Base>(int, double)>;
#define REGISTER_BASE_SUBCLASS(subclass)                         \
    static bool _registered_##subclass = BaseRegistry::Register( \
        #subclass, [](int a, double b) {                         \
            return std::unique_ptr<Base>(new subclass(a, b));    \
        });
```

The following policies for missing keys are supported:
1. `MissingKeyPolicy::exception` - throws the `std::out_of_range` from
   `unordered_map`'s `at()` when the key does not exist.
2. `MissingKeyPolicy::default_construct` - returns a default-constructed
   object when the key does not exist.
3. `MissingKeyPolicy::optional` - `Dispatch` now returns a `std::optional`
   of `Func`'s return type. This is only supported when C++17 is enabled.

*NOTE:* If the registration is taking place in a STATIC library linked to the
executable, then all of the headers with the registrations must be included
or the library must be linked with with the platform-specific whole library
flags, such as
```
-Wl,--whole-archive -lmylib -Wl,--no-whole-archive
```
on Max OS X, this is done with the flags
```
-Wl,-force_load -lmylib
```

## Template Parameters
- `Key` - The identifier type for the function map
- `Func`- The function signature type for the function map
- `MKP` - The behavior policy for what to do in the case of a missing key
- `Hash` - The hash function to use for the function map
- `KeyEqual` - The key equality function for the function map
- `Allocator` - The allocator to use for the function map

## Examples
See the examples directory for an example with CMake.
