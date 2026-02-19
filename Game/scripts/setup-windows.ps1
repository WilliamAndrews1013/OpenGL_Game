# Windows Setup Script for OpenGL Game Engine
# Run this script in PowerShell as Administrator

param(
    [string]$VcpkgRoot = "C:\dev\vcpkg",
    [string]$BuildType = "Release"
)

$ErrorActionPreference = "Stop"

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "OpenGL Game Engine - Windows Setup" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# Check if running as Administrator
$currentPrincipal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
if (-not $currentPrincipal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Write-Host "WARNING: Not running as Administrator. vcpkg integration may fail." -ForegroundColor Yellow
    Write-Host "Consider running PowerShell as Administrator for best results." -ForegroundColor Yellow
    Write-Host ""
}

# Get the project root directory
$ProjectRoot = Split-Path -Parent $PSScriptRoot
Set-Location $ProjectRoot

Write-Host "Project directory: $ProjectRoot" -ForegroundColor Green
Write-Host "Vcpkg directory: $VcpkgRoot" -ForegroundColor Green
Write-Host ""

# Function to check if a command exists
function Test-Command($Command) {
    $null = Get-Command $Command -ErrorAction SilentlyContinue
    return $?
}

# Check prerequisites
Write-Host "Checking prerequisites..." -ForegroundColor Cyan

if (-not (Test-Command "git")) {
    Write-Error "Git is not installed or not in PATH. Please install Git first."
    exit 1
}

if (-not (Test-Command "cmake")) {
    Write-Warning "CMake not found in PATH. Make sure Visual Studio 2022 is installed with C++ workload."
}

Write-Host "Prerequisites check complete." -ForegroundColor Green
Write-Host ""

# Step 1: Install vcpkg if not present
Write-Host "Step 1: Checking vcpkg..." -ForegroundColor Cyan

if (-not (Test-Path $VcpkgRoot)) {
    Write-Host "vcpkg not found. Installing..." -ForegroundColor Yellow
    
    $VcpkgParent = Split-Path -Parent $VcpkgRoot
    if (-not (Test-Path $VcpkgParent)) {
        New-Item -ItemType Directory -Path $VcpkgParent -Force | Out-Null
    }
    
    Set-Location $VcpkgParent
    git clone https://github.com/Microsoft/vcpkg.git
    
    Set-Location $VcpkgRoot
    .\bootstrap-vcpkg.bat
} else {
    Write-Host "vcpkg found at $VcpkgRoot" -ForegroundColor Green
}

# Step 2: Integrate vcpkg with Visual Studio
Write-Host ""
Write-Host "Step 2: Integrating vcpkg with Visual Studio..." -ForegroundColor Cyan

Set-Location $VcpkgRoot
$integrationResult = .\vcpkg integrate install 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Warning "vcpkg integration may have failed. You may need to run as Administrator."
} else {
    Write-Host "vcpkg integration successful." -ForegroundColor Green
}

# Step 3: Install dependencies
Write-Host ""
Write-Host "Step 3: Installing dependencies..." -ForegroundColor Cyan

$packages = @("glfw3:x64-windows", "assimp:x64-windows", "enet:x64-windows")

foreach ($package in $packages) {
    Write-Host "Installing $package..." -ForegroundColor Yellow
    .\vcpkg install $package
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Failed to install $package"
        exit 1
    }
}

Write-Host "All dependencies installed successfully." -ForegroundColor Green

# Step 4: Configure and build the project
Write-Host ""
Write-Host "Step 4: Configuring and building the project..." -ForegroundColor Cyan

Set-Location $ProjectRoot

# Create build directory
$BuildDir = Join-Path $ProjectRoot "build"
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Set-Location $BuildDir

# Configure with CMake
$VcpkgToolchain = Join-Path $VcpkgRoot "scripts\buildsystems\vcpkg.cmake"

Write-Host "Configuring CMake..." -ForegroundColor Yellow
$cmakeArgs = @(
    "..",
    "-DCMAKE_TOOLCHAIN_FILE=$VcpkgToolchain",
    "-DCMAKE_BUILD_TYPE=$BuildType"
)

cmake @cmakeArgs
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed"
    exit 1
}

# Build the project
Write-Host "Building project..." -ForegroundColor Yellow
cmake --build . --config $BuildType
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed"
    exit 1
}

Write-Host ""
Write-Host "=====================================" -ForegroundColor Green
Write-Host "Setup Complete!" -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green
Write-Host ""
Write-Host "You can now run the game:" -ForegroundColor Cyan
Write-Host "  .\build\$BuildType\GameProject.exe" -ForegroundColor White
Write-Host ""
Write-Host "Or run the tests:" -ForegroundColor Cyan
Write-Host "  .\build\$BuildType\GameTests.exe" -ForegroundColor White
Write-Host ""
Write-Host "To open in Visual Studio:" -ForegroundColor Cyan
Write-Host "  1. Open Visual Studio 2022" -ForegroundColor White
Write-Host "  2. Select 'Open a local folder'" -ForegroundColor White
Write-Host "  3. Navigate to: $ProjectRoot" -ForegroundColor White
Write-Host ""
