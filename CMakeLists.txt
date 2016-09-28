cmake_minimum_required(VERSION 3.5)
project(particle_system)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(glfw-3.2.1)
include_directories(glfw-3.2.1/include)

set(SOURCE_FILES main.cpp GLFW.cpp GLFW.h)
add_executable(particle_system ${SOURCE_FILES})

target_link_libraries(particle_system glfw ${GLFW_LIBRAIRIES})