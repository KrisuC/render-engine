cmake_minimum_required(VERSION 3.14)
project(imgui)

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

file(GLOB imgui_cpp_file "*.cpp")
file(GLOB imgui_h_file "*.h")

add_library(imgui ${imgui_cpp_file})

target_include_directories(imgui PUBLIC ${imgui_h_file})

