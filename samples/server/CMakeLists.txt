CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(coronae-server)

SET(DEPDIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

INCLUDE_DIRECTORIES(${DEPDIR} ${DEPDIR}/library/zhockon)
LINK_DIRECTORIES(${DEPDIR}/library)

ADD_EXECUTABLE(coronae-server server.cc)
TARGET_LINK_LIBRARIES(coronae-server coronae pthread)
