cmake_minimum_required(VERSION 3.10)

# 1. Project Settings
project(MyOpenGLGame VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Locate Dependencies
# -----------------------------------------------------------------------------

# --- OpenGL (System Default) ---
find_package(OpenGL REQUIRED)

# --- GLFW (Windowing) ---
# Assumes you have the GLFW source code in vendor/GLFW
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/CMakeLists.txt")
    # Hide GLFW build options from your own project list to keep it clean
    option(GLFW_BUILD_DOCS OFF)
    option(GLFW_INSTALL OFF)
    option(GLFW_BUILD_EXAMPLES OFF)
    option(GLFW_BUILD_TESTS OFF)
    
    add_subdirectory(vendor/GLFW)
else()
    message(WARNING "GLFW not found in vendor/GLFW. Ensure you downloaded the SOURCE code version.")
endif()

# 3. Setup Include Directories
# -----------------------------------------------------------------------------
# This allows you to write #include "network/Socket.h" or #include <glad/glad.h>
include_directories(
    ${PROJECT_SOURCE_DIR}/include           # Your own headers
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/stb_image
    ${PROJECT_SOURCE_DIR}/vendor/GLFW/include
)

# 4. Automate Source Discovery
# -----------------------------------------------------------------------------
# This command looks through src/ and all subfolders (core, network, graphics)
# and adds every .cpp file it finds to the build.
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# --- GLAD Specifics ---
# GLAD requires compiling its own .c file
set(GLAD_SOURCE "vendor/glad/src/glad.c")

# 5. Create the Executable
# -----------------------------------------------------------------------------
add_executable(${PROJECT_NAME} 
    ${SOURCE_FILES} 
    ${GLAD_SOURCE}
)

# 6. Link Libraries
# -----------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw            # GLFW Library
    OpenGL::GL      # System OpenGL
)

# --- Networking (Platform Specifics) ---
# If you are on Windows, you MUST link 'Ws2_32' for sockets to work.
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE Ws2_32)
endif()

# --- STB Image Implementation ---
# Defines the implementation so you don't need a separate .cpp file
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)

# 7. Asset Management
# -----------------------------------------------------------------------------
# Automatically copies the 'assets' folder to the 'bin' folder where the .exe runs
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 8. Output Directory (Optional)
# -----------------------------------------------------------------------------
# This forces the .exe to appear in a "bin" folder instead of buried in "build"
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")