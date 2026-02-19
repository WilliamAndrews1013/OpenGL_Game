# OpenGL Game Engine - Windows Setup Guide

This guide will help you set up the OpenGL Game Engine project on Windows.

## Prerequisites

- **Windows 10/11**
- **Visual Studio 2022** (Community edition is free)
  - Download from: https://visualstudio.microsoft.com/vs/community/
  - During installation, select "Desktop development with C++" workload
- **Git** for Windows: https://git-scm.com/download/win

## Quick Setup (Automated)

We provide a PowerShell script that automates most of the setup:

```powershell
# Open PowerShell as Administrator
# Navigate to the project directory
cd path\to\Game

# Run the setup script
.\scripts\setup-windows.ps1
```

This script will:
- Install vcpkg (if not present)
- Install all required dependencies
- Configure the project with CMake
- Build the project

## Manual Setup

If you prefer to set up manually, follow these steps:

### Step 1: Install vcpkg

```powershell
# Open PowerShell
# Navigate to a directory where you want to install vcpkg (e.g., C:\dev)
cd C:\dev

# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git

# Run the bootstrap script
cd vcpkg
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio (requires Administrator)
.\vcpkg integrate install
```

### Step 2: Install Dependencies

```powershell
# Install 64-bit versions of required libraries
.\vcpkg install glfw3:x64-windows
.\vcpkg install assimp:x64-windows
.\vcpkg install enet:x64-windows
```

### Step 3: Build the Project

```powershell
# Navigate to the project directory
cd path\to\Game

# Create and enter build directory
mkdir build
cd build

# Configure with CMake (replace <vcpkg-path> with your vcpkg installation path)
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\dev\vcpkg\scripts\buildsystems\vcpkg.cmake

# Build the project
cmake --build . --config Release
```

## Project Structure

```
Game/
├── assets/           # Models, textures, shaders
├── include/          # Header files
├── src/              # Source files
├── tests/            # Unit tests
├── vendor/           # Third-party libraries (GLAD, GLM, ImGui)
├── build/            # Build output (generated)
└── CMakeLists.txt    # CMake configuration
```

## Running the Game

After building, you can run the game:

```powershell
# From the build directory
.\Release\GameProject.exe
# or
.\Debug\GameProject.exe
```

Make sure the `assets` folder is accessible from the executable (it's referenced using relative paths).

## Running Tests

```powershell
# From the build directory
.\Release\GameTests.exe
# or
ctest -C Release
```

## Troubleshooting

### "Cannot find glfw3" or similar errors

Make sure vcpkg is properly integrated:
```powershell
cd C:\dev\vcpkg
.\vcpkg integrate install
```

### CMake can't find vcpkg toolchain

Use the full path to the vcpkg toolchain file:
```powershell
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\full\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
```

### Visual Studio can't find headers

1. Close Visual Studio
2. Delete the build directory
3. Re-run CMake configuration
4. Reopen the project in Visual Studio

### Game crashes on startup

- Make sure you have an OpenGL 3.3+ compatible graphics driver
- Check that the `assets` folder exists in the correct location
- Verify that shader files are accessible from the executable

## Development with Visual Studio

### Option 1: CMake Project (Recommended)

1. Open Visual Studio 2022
2. Select "Open a local folder"
3. Navigate to the project directory
4. Visual Studio will detect the CMakeLists.txt and configure automatically
5. Select your target (GameProject or GameTests) from the dropdown
6. Press F5 to build and run

### Option 2: Generate Visual Studio Solution

```powershell
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\dev\vcpkg\scripts\buildsystems\vcpkg.cmake
```

Then open `GameProject.sln` in Visual Studio.

## Dependencies Overview

| Library | Purpose | Installation |
|---------|---------|--------------|
| GLFW3 | Window creation and input handling | vcpkg |
| Assimp | 3D model loading | vcpkg |
| ENet | Network communication | vcpkg |
| GLAD | OpenGL function loading | Vendored |
| GLM | Math library | Vendored |
| ImGui | GUI library | Vendored |
| Google Test | Unit testing | FetchContent (auto) |

## Network Configuration

The game attempts to connect to a server at `10.0.0.188:3490` by default. You can modify this in `src/core/Application.cpp`.

## Additional Notes

- The project uses **C++20** standard
- The `dl` library is excluded on Windows (handled automatically in CMakeLists.txt)
- Windows-specific ENet linking is already configured in the source code

## Getting Help

If you encounter issues:
1. Check the Troubleshooting section above
2. Verify all prerequisites are installed
3. Try the automated setup script
4. Check that your graphics drivers support OpenGL 3.3+
