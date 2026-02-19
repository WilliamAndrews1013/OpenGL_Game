@echo off
REM Windows Setup Script for OpenGL Game Engine
REM This is a simple wrapper that calls the PowerShell script

echo =====================================
echo OpenGL Game Engine - Windows Setup
echo =====================================
echo.

REM Check if PowerShell is available
powershell -Command "Get-Command powershell" >nul 2>&1
if errorlevel 1 (
    echo ERROR: PowerShell is not available on this system.
    exit /b 1
)

REM Get the directory where this batch file is located
set "SCRIPT_DIR=%~dp0"

REM Run the PowerShell script
echo Running PowerShell setup script...
echo.
powershell -ExecutionPolicy Bypass -File "%SCRIPT_DIR%setup-windows.ps1" %*

if errorlevel 1 (
    echo.
    echo Setup failed with errors.
    pause
    exit /b 1
)

echo.
echo Setup completed successfully!
pause
