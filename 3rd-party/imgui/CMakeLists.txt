cmake_minimum_required(VERSION 3.14)
project(imgui)

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

add_library(imgui imconfig.h
        imgui.cpp imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        imgui_impl_glfw.h
        imgui_impl_glfw.cpp
        imgui_impl_opengl3.h
        imgui_impl_opengl3.cpp)

target_include_directories(imgui PUBLIC
        imconfig.h imgui.h imgui_internal.h imstb_rectpack.h
        imstb_textedit.h imstb_truetype.h
        imgui_impl_glfw.h
        imgui_impl_opengl3.h)

### glfw ###
include_directories("../glfw-3.3/include/")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

target_link_libraries(imgui glfw)

### glad ###
include_directories("../glad/include/")

target_link_libraries(imgui glad)
