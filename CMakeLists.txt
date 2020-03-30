# This will become a big project one day.

cmake_minimum_required(VERSION 3.14)
project(render-engine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB engine_src
        "src/app/main.cpp"
        "src/basic/*.cpp"
        "src/core/*.cpp"
        "src/components/*.cpp"
        "src/core/UniformBlock/*cpp"
        "src/core/Shadow/*cpp"
        "src/core/Mesh/*cpp"
        "src/core/Material/*cpp"
        )

add_executable(engine ${engine_src})
add_executable(test src/app/test.cpp)

target_include_directories(engine PRIVATE
        ${PROJECT_SOURCE_DIR}/src/core
        ${PROJECT_SOURCE_DIR}/src/components
        ${PROJECT_SOURCE_DIR}/src/core/UniformBlock
        ${PROJECT_SOURCE_DIR}/src/core/Shadow
        ${PROJECT_SOURCE_DIR}/src/core/Mesh
        ${PROJECT_SOURCE_DIR}/src/core/Material
        )

add_compile_definitions(ENGINE_DEBUG)

#------------------------------------------------------------------------------#
# Below is the settings for 3rd party libraries

# glm
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/glm-0.9.9.5/include/")
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/glm-0.9.9.5/)

# stb_image
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/stb_image/include/")
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/stb_image/)

# glfw
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/glfw-3.3/include/")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/glfw-3.3/)

# glad
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/glad/include/")
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/glad/)

# assimp
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/assimp-5.0/include/")
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/assimp-5.0/)

# imgui
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/imgui/")
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/imgui/)

# rapidJSON
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/rapidjson/include/")
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/rapidjson/)

target_link_libraries(imgui glad glfw)
target_link_libraries(engine assimp imgui glad glfw stb_image glm)
