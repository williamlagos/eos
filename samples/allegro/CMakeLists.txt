CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Algr-Adapter)

SET(DEPDIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bsymbolic-functions")

INCLUDE_DIRECTORIES(${DEPDIR} ${DEPDIR}/library/zhockon)
LINK_DIRECTORIES(${DEPDIR}/library)

ADD_EXECUTABLE(allegro-coronae adapter.cc)
TARGET_LINK_LIBRARIES(allegro-coronae coronae alleg)
