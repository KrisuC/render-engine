cmake_minimum_required(VERSION 3.14)
project(render-lab)

set(CMAKE_CXX_STANDARD 14)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/scenes")

add_executable(lab
        src/main.cpp
        src/shader.cpp
        src/simpleWindow.cpp
        src/model.cpp
        src/mesh.cpp)

###############################################################################
# Below is the settings for 3rd party libraries
### glm ###
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/glm-0.9.9.5/include/")
add_subdirectory(./3rd-party/glm-0.9.9.5/)

target_link_libraries(lab glm)

### stb_image ###
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/stb_image/include/")
add_subdirectory(./3rd-party/stb_image/)

target_link_libraries(lab stb_image)

### glfw ###
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/glfw-3.3/include/")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(./3rd-party/glfw-3.3/)

target_link_libraries(lab glfw)

### glad ###
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/glad/include/")
add_subdirectory(./3rd-party/glad/)

target_link_libraries(lab glad)

### assimp ###
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/assimp/include/")
add_subdirectory(./3rd-party/assimp/)

target_link_libraries(lab assimp)
