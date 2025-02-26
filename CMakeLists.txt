cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE TC-arm-rk830.cmake)

project(inav-msp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/debug.cmake)

add_subdirectory(./src)
add_subdirectory(./test)


