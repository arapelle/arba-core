
add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        check_file_tests.cpp
        copy_stream_tests.cpp
        open_file_tests.cpp
)
