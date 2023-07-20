cmake_minimum_required(VERSION 3.0.0)
project(stb_image VERSION 0.1.0)

add_library(stb_image STATIC stb_image.cpp)

target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
message(">>> stb_image include=${CMAKE_CURRENT_SOURCE_DIR}")