include(cmtk/CppLibraryTests)
include(GoogleTest)

find_package(GTest 1.14 CONFIG REQUIRED)

add_cpp_library_basic_tests(${PROJECT_NAME} GTest::gtest_main
    SOURCES
        bit/byte_swap_tests.cpp
        bit/htow_tests.cpp
        sbrm/sbrm_tests.cpp
        sbrm/sb_file_remover_tests.cpp
        string/fixed_string_tests.cpp
        string/trim_tests.cpp
        program_args_tests.cpp
        project_version_tests.cpp
)
