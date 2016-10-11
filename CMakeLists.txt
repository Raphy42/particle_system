cmake_minimum_required(VERSION 3.5)
enable_language(CXX)
project(particle_system)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -framework OpenCL -framework Cocoa -framework OpenGL -framework IOKit")

find_package(OpenCL REQUIRED)
include_directories(${OPENCL_INCLUDE_DIR})

add_subdirectory(glfw-3.2.1)
include_directories(glfw-3.2.1/include)
include_directories(include)

set(SOURCE_FILES main.cpp src/opengl/GLFW.cpp src/opengl/GLFW.h src/opencl/SharedContext.cpp src/opencl/SharedContext.h src/opencl/utils.h src/Core.cpp src/Core.h src/opencl/utils.cpp src/opencl/cl.h)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(particle_system ${SOURCE_FILES})

target_link_libraries(particle_system glfw ${GLFW_LIBRAIRIES} ${OPENCL_LIBRAIRIES})