@echo off
REM setup.bat - Prerequisites check and setup script for blanceCar project
echo ========================================
echo   blanceCar Project Setup
echo ========================================
echo.

REM Check for ARM GCC toolchain
echo Checking for ARM GCC toolchain...
where arm-none-eabi-gcc >nul 2>nul
if %errorlevel% equ 0 (
    echo [OK] arm-none-eabi-gcc found
    arm-none-eabi-gcc --version | findstr /C:"arm-none-eabi-gcc"
) else (
    echo [ERROR] arm-none-eabi-gcc not found in PATH
    echo Please install ARM GNU Toolchain from:
    echo https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
)

echo.

REM Check for CMake
echo Checking for CMake...
where cmake >nul 2>nul
if %errorlevel% equ 0 (
    echo [OK] CMake found
    cmake --version | findstr /C:"cmake version"
) else (
    echo [WARNING] CMake not found in PATH
    echo CMake 3.22 or higher is required for this project.
    echo.
    echo Installation options:
    echo 1. Download from https://cmake.org/download/
    echo 2. Install via Chocolatey: choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'
    echo 3. Install via Scoop: scoop install cmake
    echo.
    echo After installation, restart your terminal or run: refreshenv
)

echo.

REM Check for Ninja
echo Checking for Ninja...
where ninja >nul 2>nul
if %errorlevel% equ 0 (
    echo [OK] Ninja found
    ninja --version
) else (
    echo [INFO] Ninja not found but can be installed
    echo Ninja is optional but recommended for faster builds.
    echo Install via: choco install ninja OR scoop install ninja
)

echo.

REM Check for Git
echo Checking for Git...
where git >nul 2>nul
if %errorlevel% equ 0 (
    echo [OK] Git found
    git --version
) else (
    echo [INFO] Git not found
    echo Git is used for version control. Install from https://git-scm.com/
)

echo.
echo ========================================
echo   Next Steps
echo ========================================
echo.
echo 1. Install missing prerequisites (see above)
echo 2. Run the following commands to build:
echo.
echo    cmake --preset Debug
echo    cmake --build --preset Debug
echo.
echo 3. For VS Code users, install these extensions:
echo    - C/C++ (Microsoft)
echo    - CMake Tools (Microsoft)
echo    - Cortex-Debug (marus25)
echo.
echo 4. Open this folder in VS Code
echo    The CMake Tools extension will help configure and build.
echo.
echo 5. Flash the generated .elf or .bin file to your STM32 board.
echo.
echo For more details, see README.md
echo ========================================
pause