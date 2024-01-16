find_or_fetch_GTest()

add_cpp_library_basic_tests(GTest::gtest_main
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    SOURCES
        bit/byte_swap_tests.cpp
        bit/htow_tests.cpp
        chrono/format_time_point_tests.cpp
        chrono/stopwatch_tests.cpp
        debug/debug_tests.cpp
        design_pattern/singleton_tests.cpp
        io/check_file_tests.cpp
        io/copy_stream_tests.cpp
        io/open_file_tests.cpp
        sbrm/sbrm_tests.cpp
        sbrm/sb_file_remover_tests.cpp
        string/fixed_string_tests.cpp
        string/string_tokenizer_tests.cpp
        string/trim_tests.cpp
        template/default_type_tests.cpp
        template/request_type_tests.cpp
        #
        algorithm_tests.cpp
        uuid_tests.cpp
        hash_tests.cpp
        random_tests.cpp
        span_tests.cpp
        type_traits_tests.cpp
        project_version_tests.cpp
)

add_subdirectory(plugin_tests)
