
add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        default_type_tests.cpp
        request_type_tests.cpp
)
