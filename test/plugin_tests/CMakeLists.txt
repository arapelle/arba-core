add_subdirectory(rng_interface)
add_subdirectory(rng)
get_target_property(rng_dir rng RUNTIME_OUTPUT_DIRECTORY)

add_cpp_library_test(${PROJECT_NAME}-plugin_tests GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        plugin_tests.cpp
)
target_link_libraries(${PROJECT_NAME}-plugin_tests PUBLIC rng_interface)
target_compile_definitions(${PROJECT_NAME}-plugin_tests PUBLIC PLUGIN_DIR="${rng_dir}")
