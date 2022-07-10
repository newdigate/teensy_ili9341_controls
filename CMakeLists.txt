cmake_minimum_required(VERSION 3.22)
project(teensy_ili9341_controls)

set(CMAKE_CXX_STANDARD 14)

find_package(teensy_x86_stubs)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})

add_definitions(-DARDUINO=146)
add_library(teensy_ili9341_controls
        ILI9341_t3_Controls.cpp
        ILI9341_t3_Controls.h)
