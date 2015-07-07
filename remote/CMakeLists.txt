CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(coronae-core)

ADD_LIBRARY(coronae SHARED coronae.cc)
ADD_LIBRARY(coronae-zkn SHARED coronae.cc coronae-zkn.cc)
TARGET_LINK_LIBRARIES(coronae json jsonrpc)
TARGET_LINK_LIBRARIES(coronae-zkn zhockon json jsonrpc)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../library)
