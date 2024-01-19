cmake_minimum_required(VERSION 3.27)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    # set MSVC utf-8 build
    add_compile_options(/utf-8)
endif ()

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_subdirectory(01-HelloWindow)
add_subdirectory(02-Triangle)
add_subdirectory(03-Shader)
