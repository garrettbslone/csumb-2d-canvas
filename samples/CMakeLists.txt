cmake_minimum_required(VERSION 3.16)
project(mb2dc)

set(CMAKE_CXX_STANDARD 14)

add_executable(sample main.cpp)

target_link_libraries(sample mb2dc)