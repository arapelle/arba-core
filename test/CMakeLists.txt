include(cmtk/CppLibraryTests)
include(GoogleTest)

find_package(GTest 1.14 CONFIG REQUIRED)

add_cpp_library_basic_tests(${PROJECT_NAME} GTest::gtest_main
    SOURCES
        program_args_tests.cpp
        project_version_tests.cpp
)

add_subdirectory(bit)
add_subdirectory(sbrm)
add_subdirectory(string)
