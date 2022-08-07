cmake_minimum_required(VERSION 3.23)
project(mb2dc)

set(CMAKE_CXX_STANDARD 14)

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
endif()

set(CORE_SRCS src/core/window.cpp src/core/canvas.cpp src/core/input.cpp)

add_subdirectory(ext/glfw)

add_library(mb2dc SHARED ${CORE_SRCS})

target_include_directories(mb2dc PUBLIC inc ext ext/glfw/include ext/glm)
target_link_directories(mb2dc PRIVATE ${CMAKE_SOURCE_DIR}/ext/glfw/src)
target_link_libraries(mb2dc PRIVATE glfw)

add_subdirectory(samples)