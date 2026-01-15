cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()

project(OpenGLGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW (still uses MakeAvailable as it works)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)
FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
  FetchContent_Populate(glm)
endif()

# GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/libigl/libigl-glad.git
  GIT_TAG        09b4969c56779f7ddf8e6176ec1873184aec890f
)
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
  FetchContent_Populate(glad)
endif()

add_executable(OpenGLGame
    src/main.cpp
    src/core/Application.cpp
    src/core/Window.cpp
    ${glad_SOURCE_DIR}/src/glad.c
)

target_include_directories(OpenGLGame PRIVATE 
    src
    ${glm_SOURCE_DIR}
    ${glad_SOURCE_DIR}/include
)

target_link_libraries(OpenGLGame PRIVATE glfw)

