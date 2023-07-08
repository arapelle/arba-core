cmake_minimum_required(VERSION 3.18)

# CMAKE INCLUDES

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/external/cmake/)
include(CMakePrintHelpers)
include(cmtk/CppLibraryProject)

# CONFIGURATION

disable_in_source_build()
set_build_type_ifndef()

# C++ PROJECT

set(PROJECT_NAME "arba-core")
set(project_namespace "arba")
set(project_base_name "core")
set(PROJECT_VERSION 0.14.0)
project(${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "C++ core functions."
        HOMEPAGE_URL "https://github.com/arapelle/core"
        LANGUAGES CXX)
message(STATUS "PROJECT : ${PROJECT_NAME} ${PROJECT_VERSION} ${CMAKE_BUILD_TYPE}")
include(CTest)

# C++ LIBRARY

## Generated/Configured headers:
generate_version_macro_header(generated_version_header
                              ${project_namespace}_${project_base_name}
                              ${project_namespace}/${project_base_name}/version.hpp)

## Headers:
set(headers
    include/arba/core/intrusive_ptr.hpp
    include/arba/core/string.hpp
    include/arba/core/sbrm.hpp
    include/arba/core/htow.hpp
    include/arba/core/byte_swap.hpp
    include/arba/core/uuid.hpp
    include/arba/core/io/check_file.hpp
    include/arba/core/io/open_file.hpp
    include/arba/core/io/copy_stream.hpp
    include/arba/core/type_helpers.hpp
    include/arba/core/string_tokenizer.hpp
    include/arba/core/hash.hpp
)

## Sources:
set(sources
    src/hash.cpp
    src/uuid.cpp
    src/io/copy_stream.cpp
    src/io/check_file.cpp
    src/io/open_file.cpp
)

## Add C++ library
add_cpp_library(
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    BUILD_SHARED OPTION
    BUILD_STATIC OPTION
    HEADERS ${headers} ${generated_version_header}
    SOURCES ${sources}
    CXX_STANDARD 20
)

## Install C++ library:
install_cpp_library(
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    EXPORT ${PROJECT_NAME}-targets
)

## Install project package
install_library_package(${PROJECT_NAME}
                        INPUT_PACKAGE_CONFIG_FILE CMake-package-config.cmake.in
)
install_uninstall_script(${PROJECT_NAME})

## Add examples:
option_or_set_ifdef(BUILD_${PROJECT_NAME}_EXAMPLES "Build ${PROJECT_NAME} examples or not" OFF BUILD_arba-arba_EXAMPLES)
if(BUILD_${PROJECT_NAME}_EXAMPLES)
    add_subdirectory(example)
endif()

## Add tests:
option_or_set_ifdef(BUILD_${PROJECT_NAME}_TESTS "Build ${PROJECT_NAME} tests or not" OFF BUILD_arba-arba_TESTS)
option(BUILD_${PROJECT_NAME}_TESTS "Build ${PROJECT_NAME} tests or not" OFF)
if(BUILD_${PROJECT_NAME}_TESTS AND BUILD_TESTING)
    add_subdirectory(test)
endif()
