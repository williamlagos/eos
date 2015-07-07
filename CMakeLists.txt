cmake_minimum_required(VERSION 2.8)

project(dinamo)

find_package(SDL)
find_package(OpenGL)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libjsonrpc/json/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libjsonrpc/jsonrpc/include)

set(WITH_EXAMPLES OFF)
set(WITH_STATIC_LIB ON)
#set(WITH_STATIC_EXE ON)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/library)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libssh)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libjsonrpc)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libwiring)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/remote)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/library/GLUT)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/library/GLUI)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/library/Box2D)

