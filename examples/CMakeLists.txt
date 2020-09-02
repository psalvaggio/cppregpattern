set(SRCS derived_classes.cpp)
add_library(examples_shared SHARED ${SRCS})
target_compile_features(examples_shared PUBLIC cxx_std_17)
target_link_libraries(examples_shared PUBLIC cppregpattern::cppregpattern)

#if (APPLE)
#  target_link_options(REGISTRY_EXAMPLE_LIB -Wl,-force_load registry_examples)
#else()
#  set(REGISTRY_EXAMPLE_LIB
#      -Wl,--whole-archive registry_examples -Wl,--no-whole-archive)
#endif()

add_executable(example_shared example.cpp)
target_compile_features(example_shared PUBLIC cxx_std_17)
target_link_libraries(example_shared examples_shared)
