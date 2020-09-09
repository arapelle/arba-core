cmake_minimum_required(VERSION 3.16)

project(basic_cmake_project)

add_executable(${PROJECT_NAME} main.cpp)
# Find package core:
find_package(core 0.3.2 CONFIG REQUIRED)
# Use core release shared target:
target_link_libraries(${PROJECT_NAME} PRIVATE arba::core)
# Use core release static target:
#target_link_libraries(${PROJECT_NAME} PRIVATE arba::core-static)
