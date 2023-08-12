
add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        isptr_with_custom_counter_tests.cpp
        isptr_with_core_counter_tests.cpp
        make_isptr_tests.cpp
)
