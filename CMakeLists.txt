cmake_minimum_required(VERSION 3.16)
project(MyEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

add_subdirectory(external/glfw)
add_subdirectory(external/glm)

add_executable(MyEngine
    src/main.cpp
    src/engine/Engine.cpp
    src/graphics/Renderer.cpp
    src/graphics/Shader.cpp
    src/graphics/Mesh.cpp
    src/math/Transform.cpp
    src/math/Camera.cpp
    external/glad/src/glad.c
)

target_include_directories(MyEngine PRIVATE
    src
    external/glad/include
)

target_link_libraries(MyEngine
    glfw
    opengl32
    glm::glm
)
