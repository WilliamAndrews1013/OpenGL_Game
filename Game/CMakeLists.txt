# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# Define project name and optionally the language
project(GameProject C CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
enable_testing()

# Find packages
find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Core library sources (for testing)
set(CORE_SOURCES
    src/core/Application.cpp
    src/core/Window.cpp
    src/game/LocalPlayer.cpp
    src/network/NetworkClient.cpp
    src/graphics/Model.cpp
    src/ecs/RenderSystem.cpp
    src/ecs/InputSystem.cpp
    src/ecs/CameraSystem.cpp
)

# Vendor sources
set(VENDOR_SOURCES
    vendor/glad/src/glad.c
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

# Core library for testing
add_library(GameCore STATIC ${CORE_SOURCES})

target_include_directories(GameCore PUBLIC
    include
    vendor
    vendor/imgui
    vendor/imgui/backends
)

target_link_libraries(GameCore PUBLIC 
    glfw 
    enet 
    assimp::assimp 
    OpenGL::GL
)

if (NOT WIN32)
    target_link_libraries(GameCore PUBLIC dl)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(GameCore PRIVATE /W4 /WX)
else()
    target_compile_options(GameCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Game executable
add_executable(GameProject 
    src/main.cpp
    ${VENDOR_SOURCES}
)

target_include_directories(GameProject PRIVATE
    include
    vendor
    vendor/imgui
    vendor/imgui/backends
)

target_link_libraries(GameProject PRIVATE 
    GameCore
)

# Tests
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

add_executable(GameTests ${TEST_SOURCES})

target_include_directories(GameTests PRIVATE
    include
    vendor
)

target_link_libraries(GameTests PRIVATE
    GameCore
    GTest::gtest_main
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(GameTests)
