cmake_minimum_required(VERSION 3.16)

project(basic_cmake_project)

add_executable(${PROJECT_NAME} main.cpp)
# Find package core:
find_package(core 0.3.0 CONFIG REQUIRED)
# Use core release shared target:
target_link_libraries(${PROJECT_NAME} PRIVATE core::core)
# Use core release static target:
#target_link_libraries(${PROJECT_NAME} PRIVATE core::core-static)
