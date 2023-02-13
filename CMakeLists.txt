cmake_minimum_required(VERSION 3.14)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 17)

add_executable(Mandelbrot main.cpp shader.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

add_subdirectory(glad/cmake)
glad_add_library(glad_gl_core33 SHARED API gl:core=3.3)

add_subdirectory(glm)

target_link_libraries(Mandelbrot ${GLEW_LIBRARIES} glfw glad_gl_core33 glm)
target_compile_definitions(Mandelbrot PUBLIC SOURCE_PATH=\"${PROJECT_SOURCE_DIR}\")