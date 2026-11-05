# EOS SDK CMake Cross-Compilation Toolchain: Arch Linux ARMv6 hard-float (Raspberry Pi 1 / Zero / Zero W)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armv6l)

set(_EOS_XTOOLS_ROOT "/usr/local/x-tools-armv6h/x-tools6h/arm-unknown-linux-gnueabihf")

if(EXISTS "${_EOS_XTOOLS_ROOT}/bin/arm-unknown-linux-gnueabihf-gcc")
    set(CMAKE_C_COMPILER "${_EOS_XTOOLS_ROOT}/bin/arm-unknown-linux-gnueabihf-gcc")
    set(CMAKE_CXX_COMPILER "${_EOS_XTOOLS_ROOT}/bin/arm-unknown-linux-gnueabihf-g++")
    set(CMAKE_FIND_ROOT_PATH "${_EOS_XTOOLS_ROOT}/arm-unknown-linux-gnueabihf/sysroot")
else()
    set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
    set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
endif()

set(CMAKE_C_FLAGS_INIT "-march=armv6zk -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard")
set(CMAKE_CXX_FLAGS_INIT "-march=armv6zk -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
