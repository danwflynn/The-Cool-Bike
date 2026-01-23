cmake_minimum_required(VERSION 3.16)
project(MyEngine)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/glfw)

add_executable(MyEngine
    src/main.cpp
    src/engine/Engine.cpp
)

target_include_directories(MyEngine PRIVATE
    src
    external/glfw/include
)

target_link_libraries(MyEngine glfw opengl32)
