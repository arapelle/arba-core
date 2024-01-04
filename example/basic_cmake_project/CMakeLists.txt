cmake_minimum_required(VERSION 3.26)

project(basic_cmake_project)

add_executable(${PROJECT_NAME} basic_cmake_project.cpp)
add_executable(${PROJECT_NAME}-static basic_cmake_project.cpp)
# Find package arba-core:
find_package(arba-core 0.25.0 CONFIG REQUIRED)
# Use arba-core release shared target:
target_link_libraries(${PROJECT_NAME} PRIVATE arba-core)
# Use arba-core release static target:
target_link_libraries(${PROJECT_NAME}-static PRIVATE arba-core-static)
