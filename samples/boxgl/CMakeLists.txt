project(BoxGL)

cmake_minimum_required(VERSION 2.8)

if(WIN32)
 set(GLUT_INCLUDE_DIR include/)
 set(GLUT_glut_LIBRARY lib/glut32)
endif()

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(GRAPHICS_LIBRARIES ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} Box2D json)
set(GRAPHICS_SOURCES elements.cpp physics.cpp graphics.cpp main.cpp)

add_executable(graphics ${GRAPHICS_SOURCES})
target_link_libraries(graphics ${GRAPHICS_LIBRARIES})
