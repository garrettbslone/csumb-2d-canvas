cmake_minimum_required(VERSION 3.16)
project(mb2dc)

set(CMAKE_CXX_STANDARD 20)

add_executable(rect_and_texture_circle rect_and_texture_circle.cpp)
add_executable(checkers checkers.cpp)
add_executable(basic_text basic_text.cpp)

target_link_libraries(rect_and_texture_circle mb2dc)
target_link_libraries(checkers mb2dc)
target_link_libraries(basic_text mb2dc)