cmake_minimum_required(VERSION 3.14)
project(stb_image)

add_library(stb_image include/stb_image.h src/stb_image.cpp)
target_include_directories(stb_image PUBLIC include/)