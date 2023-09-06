
add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        trinum_version_tests.cpp
        constexpr_trinum_version_tests.cpp
)
