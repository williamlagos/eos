cmake_minimum_required(VERSION 2.8)

project(dinamo)

find_package(SDL)
find_package(OpenGL)
find_package(Box2D)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH library)
