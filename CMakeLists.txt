cmake_minimum_required(VERSION 3.14)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 17)

add_executable(Mandelbrot main.cpp shader.h)

include_directories("/usr/include/GLFW" , "/usr/include/glm")
target_link_libraries(Mandelbrot glfw)

add_library("glad" "/usr/src/glad.c")
target_include_directories("glad" PRIVATE "/usr/include")
target_link_libraries(Mandelbrot "glad" ${CMAKE_DL_LIBS})