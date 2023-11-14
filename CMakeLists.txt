cmake_minimum_required(VERSION 3.26)

# CMAKE INCLUDES

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/external/cmake/)
include(CMakePrintHelpers)
include(cmtk/CppLibraryProject)

# CONFIGURATION

disable_in_source_build()
set_build_type_ifndef()

macro(set_PROJECT_SEMANTIC_VERSION)
    set(PROJECT_SEMANTIC_VERSION "${PROJECT_VERSION}")
    if(PROJECT_VERSION_PRE_RELEASE)
        string(APPEND PROJECT_SEMANTIC_VERSION "-${PROJECT_VERSION_PRE_RELEASE}")
    endif()
    if(PROJECT_VERSION_BUILD_METADATA)
        string(APPEND PROJECT_SEMANTIC_VERSION "+${PROJECT_VERSION_BUILD_METADATA}")
    endif()
endmacro()

# C++ PROJECT

set(PROJECT_NAME "arba-core")
set(PROJECT_NAMESPACE "arba")
set(PROJECT_BASE_NAME "core")
set(PROJECT_VERSION 0.23.0)
set(PROJECT_VERSION_PRE_RELEASE "dev")
string(TIMESTAMP PROJECT_VERSION_BUILD_METADATA "%Y%m%d-%H%M%S" UTC)
set_PROJECT_SEMANTIC_VERSION()

project(${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "C++ core functions."
        HOMEPAGE_URL "https://github.com/arapelle/arba-core"
        LANGUAGES CXX)
message(STATUS "PROJECT : ${PROJECT_NAME} ${PROJECT_VERSION} ${CMAKE_BUILD_TYPE}")
include(CTest)

# Add external dependencies (submodules and fetch/find packages)

add_subdirectory(external)

# C++ LIBRARY

## Generated/Configured headers:
configure_headers(configured_headers
    FILES
        include/${PROJECT_NAMESPACE}/${PROJECT_BASE_NAME}/version.hpp.in
)

## Headers:
set(headers
    include/arba/core/algorithm.hpp
    include/arba/core/byte_swap.hpp
    include/arba/core/chrono/duration.hpp
    include/arba/core/chrono/format_time_point.hpp
    include/arba/core/chrono/stopwatch.hpp
    include/arba/core/compile_time_error.hpp
    include/arba/core/debug/assert.hpp
    include/arba/core/debug/assert_msg.hpp
    include/arba/core/debug/breakpoint.hpp
    include/arba/core/design_pattern/singleton.hpp
    include/arba/core/hash.hpp
    include/arba/core/htow.hpp
    include/arba/core/io/check_file.hpp
    include/arba/core/io/copy_stream.hpp
    include/arba/core/io/open_file.hpp
    include/arba/core/plugin.hpp
    include/arba/core/random.hpp
    include/arba/core/sbrm.hpp
    include/arba/core/span.hpp
    include/arba/core/string/fixed_string.hpp
    include/arba/core/string.hpp
    include/arba/core/string/string_conversion.hpp
    include/arba/core/string_tokenizer.hpp
    include/arba/core/template/default_type.hpp
    include/arba/core/template/exception_policy.hpp
    include/arba/core/template/request_type.hpp
    include/arba/core/template/thread_policy.hpp
    include/arba/core/type_traits.hpp
    include/arba/core/uuid.hpp
    include/arba/core/version/concepts/semantic_version.hpp
    include/arba/core/version/concepts/tri_version.hpp
    include/arba/core/version/_private/extract_semantic_version.hpp
    include/arba/core/version/_private/extract_tri_version.hpp
    include/arba/core/version/is_compatible_with.hpp
    include/arba/core/version/semantic_version.hpp
    include/arba/core/version/tri_version.hpp
    include/arba/core/version/tri_version_tag.hpp
)

## Sources:
set(sources
    src/chrono/format_time_point.cpp
    src/chrono/stopwatch.cpp
    src/debug/assert.cpp
    src/debug/assert_msg.cpp
    src/hash.cpp
    src/io/check_file.cpp
    src/io/copy_stream.cpp
    src/io/open_file.cpp
    src/plugin.cpp
    src/random.cpp
    src/span.cpp
    src/uuid.cpp
)

## Add C++ library
add_cpp_library(
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    BUILD_SHARED BUILD_arba-arba
    BUILD_STATIC BUILD_arba-arba-static
    HEADERS ${headers} ${configured_headers}
    SOURCES ${sources}
    CXX_STANDARD 20
)

## Link C++ targets:
if(NOT WIN32)
    find_library(dl-static_path libdl.a REQUIRED)
    cpp_library_link_libraries(
        SHARED ${PROJECT_NAME}
        STATIC ${PROJECT_NAME}-static
        PRIVATE ${dl-static_path}
    )
endif()

## Install C++ library:
install_cpp_library(
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    EXPORT ${PROJECT_NAME}-targets
)

## Install project package
install_library_package(${PROJECT_NAME} INPUT_PACKAGE_CONFIG_FILE cmake/config/package-config.cmake.in)
install_uninstall_script(${PROJECT_NAME} FILENAME uninstall.cmake)

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
