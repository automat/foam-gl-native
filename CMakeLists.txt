#pureley for ide inspection
cmake_minimum_required(VERSION 2.8.4)
project(foam-gl-native)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#glfw
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

#glew
pkg_search_module(GLEW REQUIRED glew)
include_directories(${GLEW_INCLUDE_DIRS})

#v8 node nan
include_directories(/usr/local/include/node/ node_modules/nan)

set(SOURCE_FILES src/bindings/main.cpp src/bindings/gl/gl.cpp src/bindings/gl/glConsts.cpp src/bindings/gl/glfw.cpp)
add_executable(node-foam-gl ${SOURCE_FILES})