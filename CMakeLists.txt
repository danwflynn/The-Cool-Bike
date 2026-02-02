cmake_minimum_required(VERSION 3.16)
project(MyEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/assimp)

add_library(MyEngineCore
    src/engine/Engine.cpp

    src/graphics/Renderer.cpp
    src/graphics/Shader.cpp
    src/graphics/Mesh.cpp
    src/graphics/Model.cpp

    src/math/Transform.cpp
    src/math/Camera.cpp

    src/entities/Bike.cpp
    src/entities/WheelHitbox.cpp
)

target_include_directories(MyEngineCore PUBLIC
    src
    external/glad/include
)

target_link_libraries(MyEngineCore
    glfw
    glm::glm
    assimp
)

add_executable(MyEngine
    src/main.cpp
    external/glad/src/glad.c
)

target_link_libraries(MyEngine
    MyEngineCore
    opengl32
)

add_executable(MyEngineTests
    test/math/TransformTests.cpp
    test/math/CameraTests.cpp
    test/entities/WheelHitboxTests.cpp
)

target_compile_definitions(MyEngineTests PRIVATE
    GLM_ENABLE_EXPERIMENTAL
)

target_link_libraries(MyEngineTests
    MyEngineCore
    gtest_main
)

enable_testing()

include(GoogleTest)
gtest_discover_tests(MyEngineTests)
