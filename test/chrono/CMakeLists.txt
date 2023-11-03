
add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        format_time_point_tests.cpp
        stopwatch_tests.cpp
)
