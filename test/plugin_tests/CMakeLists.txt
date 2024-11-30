add_subdirectory(concat_interface)
add_subdirectory(concat)
get_target_property(concat_dir arba_core_concat RUNTIME_OUTPUT_DIRECTORY)

add_cpp_library_test(${PROJECT_NAME}-plugin_tests GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        plugin_tests.cpp
)
target_link_libraries(${PROJECT_NAME}-plugin_tests PUBLIC arba_core_concat_interface)
target_compile_definitions(${PROJECT_NAME}-plugin_tests PUBLIC PLUGIN_DIR="${concat_dir}")

add_cpp_library_test(${PROJECT_NAME}-unsafe_plugin_tests GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        unsafe_plugin_tests.cpp
)
target_link_libraries(${PROJECT_NAME}-unsafe_plugin_tests PUBLIC arba_core_concat_interface)
target_compile_definitions(${PROJECT_NAME}-unsafe_plugin_tests PUBLIC PLUGIN_DIR="${concat_dir}")
