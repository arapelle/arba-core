
add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        extract_semantic_version_tests.cpp
        extract_tri_version_tests.cpp
        tri_version_tests.cpp
        tri_version_tag_tests.cpp
        semantic_version_tests.cpp
)
