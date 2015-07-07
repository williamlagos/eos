CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(coronae-core)

add_library(coronae SHARED 
		coronae.cc 
		extensor.cc
		parser.cc)
add_library(coronae_static STATIC
		coronae.cc
		extensor.cc
		parser.cc)
target_link_libraries(coronae json jsonrpc ssh_shared)
target_link_libraries(coronae_static json_static jsonrpc_static ssh_static) 

add_executable(hub hub.cc)
target_link_libraries(hub coronae)

#if(WITH_STATIC_EXE)
#	set(BUILD_SHARED_LIBRARIES OFF)
#	set(CMAKE_EXE_LINKER_FLAGS "-static")
#	add_executable(hub_static hub.cc)
#	target_link_libraries(hub_static coronae_static)
#endif(WITH_STATIC_EXE)
