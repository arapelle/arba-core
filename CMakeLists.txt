cmake_minimum_required(VERSION 3.26)

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
set(PROJECT_VERSION 0.15.0)
project(${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "C++ core functions."
        HOMEPAGE_URL "https://github.com/arapelle/arba-core"
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
    include/arba/core/byte_swap.hpp
    include/arba/core/hash.hpp
    include/arba/core/htow.hpp
    include/arba/core/io/check_file.hpp
    include/arba/core/io/copy_stream.hpp
    include/arba/core/io/open_file.hpp
    include/arba/core/memory/concepts.hpp
    include/arba/core/memory/intrusive_ref_counter.hpp
    include/arba/core/memory/intrusive_shared_ptr.hpp
    include/arba/core/memory/intrusive_weak_ptr.hpp
    include/arba/core/plugin.hpp
    include/arba/core/random.hpp
    include/arba/core/sbrm.hpp
    include/arba/core/span.hpp
    include/arba/core/string.hpp
    include/arba/core/string_tokenizer.hpp
    include/arba/core/template/maythrow.hpp
    include/arba/core/template/request_type.hpp
    include/arba/core/uuid.hpp
)

## Sources:
set(sources
    src/hash.cpp
    src/io/check_file.cpp
    src/io/copy_stream.cpp
    src/io/open_file.cpp
    src/plugin.cpp
    src/span.cpp
    src/uuid.cpp
)

## Add C++ library
add_cpp_library(
    SHARED ${PROJECT_NAME}
    STATIC ${PROJECT_NAME}-static
    BUILD_SHARED BUILD_arba-arba
    BUILD_STATIC BUILD_arba-arba-static
    HEADERS ${headers} ${generated_version_header}
    SOURCES ${sources}
    CXX_STANDARD 20
)

## Link C++ targets:
if(NOT WIN32)
    find_library(dl-static_path libdl.a REQUIRED)
    cpp_library_link_libraries(
        SHARED ${PROJECT_NAME}
        STATIC ${PROJECT_NAME}-static
        PUBLIC ${dl-static_path}
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
