cmake_minimum_required(VERSION 3.6)
project(printf C)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*)

include_directories(include)

add_executable(printf_tests
        test.c
        ${SOURCES})
