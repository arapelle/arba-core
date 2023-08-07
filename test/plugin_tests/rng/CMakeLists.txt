add_library(rng SHARED rng.cpp)
target_compile_features(rng PUBLIC cxx_std_20)
target_link_libraries(rng PUBLIC rng_interface)
set_property(TARGET rng PROPERTY POSITION_INDEPENDENT_CODE 1)
