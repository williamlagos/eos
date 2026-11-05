# EOS SDK CMake Cross-Compilation Toolchain: Arch Linux ARMv8 64-bit (Raspberry Pi 3 / 4 / 5, ARM Handhelds & Cloud)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(_EOS_XTOOLS_ROOT "/usr/local/x-tools-armv8/x-tools8/aarch64-unknown-linux-gnu")

if(EXISTS "${_EOS_XTOOLS_ROOT}/bin/aarch64-unknown-linux-gnu-gcc")
    set(CMAKE_C_COMPILER "${_EOS_XTOOLS_ROOT}/bin/aarch64-unknown-linux-gnu-gcc")
    set(CMAKE_CXX_COMPILER "${_EOS_XTOOLS_ROOT}/bin/aarch64-unknown-linux-gnu-g++")
    set(CMAKE_FIND_ROOT_PATH "${_EOS_XTOOLS_ROOT}/aarch64-unknown-linux-gnu/sysroot")
else()
    set(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
    set(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
