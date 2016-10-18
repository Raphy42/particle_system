cmake_minimum_required(VERSION 3.5)
enable_language(CXX)
project(particle_system)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -framework OpenCL -framework Cocoa -framework OpenGL -framework IOKit")

find_package(OpenCL REQUIRED)
include_directories(${OPENCL_INCLUDE_DIR})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

add_subdirectory(glfw-3.2.1)
include_directories(glfw-3.2.1/include)
include_directories(include)

set(SOURCE_FILES main.cpp utils/FileLogger.cpp utils/FileLogger.h proxies/GLFW.cpp proxies/GLFW.h proxies/ProxyInterface.h proxies/FreeType.cpp proxies/FreeType.h utils/FrameCounter.cpp utils/FrameCounter.h opengl/Program.cpp opengl/Program.h proxies/OpenCL.cpp proxies/OpenCL.h)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(particle_system ${SOURCE_FILES})

target_link_libraries(particle_system
        glfw ${GLFW_LIBRAIRIES}
        freetype ${FREETYPE_LIBRAIRIES}
        ${OPENCL_LIBRAIRIES})