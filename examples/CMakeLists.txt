add_library(registry_examples base_classes.cpp derived_classes.cpp)
IF (APPLE)
  set(REGISTRY_EXAMPLE_LIB -Wl,-force_load registry_examples)
else()
  set(REGISTRY_EXAMPLE_LIB
      -Wl,--whole-archive registry_examples -Wl,--no-whole-archive)
endif()

add_executable(example example.cpp)
target_link_libraries(example ${REGISTRY_EXAMPLE_LIB})
