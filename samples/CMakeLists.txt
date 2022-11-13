cmake_minimum_required(VERSION 3.16)
project(mb2dc)

set(CMAKE_CXX_STANDARD 17)

add_executable(rect_and_texture_circle rect_and_texture_circle.cpp)
add_executable(checkers checkers.cpp)

target_link_libraries(rect_and_texture_circle mb2dc)
target_link_libraries(checkers mb2dc)