cmake_minimum_required(VERSION 3.14)
project(imgui)

add_library(imgui imconfig.h
        imgui.cpp imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        examples/imgui_impl_glfw.h
        examples/imgui_impl_glfw.cpp
        examples/imgui_impl_opengl3.h
        examples/imgui_impl_opengl3.cpp)

target_include_directories(imgui PUBLIC
        imconfig.h imgui.h imgui_internal.h imstb_rectpack.h
        imstb_textedit.h imstb_truetype.h
        examples/imgui_impl_glfw.h
        examples/imgui_impl_opengl3.h)