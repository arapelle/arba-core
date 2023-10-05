find_or_fetch_GTest()

add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        project_version_tests.cpp
        algorithm_tests.cpp
        uuid_tests.cpp
        hash_tests.cpp
        byte_swap_tests.cpp
        htow_tests.cpp
        string_tokenizer_tests.cpp
        string_tests.cpp
        sbrm_tests.cpp
        random_tests.cpp
        span_tests.cpp
        type_traits_tests.cpp
)

add_subdirectory(debug)
add_subdirectory(io)
add_subdirectory(plugin_tests)
add_subdirectory(template)
add_subdirectory(version)
