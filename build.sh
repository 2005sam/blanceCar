#!/bin/bash
# build.sh - Build script for blanceCar project (Linux/macOS)

set -e

echo "========================================"
echo "   blanceCar Project Build"
echo "========================================"
echo

# Check for required tools
command -v arm-none-eabi-gcc >/dev/null 2>&1 || { 
    echo "ERROR: arm-none-eabi-gcc not found in PATH"
    echo "Please install ARM GNU Toolchain from:"
    echo "https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm"
    exit 1
}

command -v cmake >/dev/null 2>&1 || {
    echo "ERROR: CMake not found in PATH"
    echo "Please install CMake 3.22 or higher from:"
    echo "https://cmake.org/download/"
    exit 1
}

command -v ninja >/dev/null 2>&1 || {
    echo "WARNING: Ninja not found, using make instead"
    GENERATOR="Unix Makefiles"
}

GENERATOR=${GENERATOR:-"Ninja"}

echo "Using generator: $GENERATOR"
echo

# Configure and build
echo "Configuring project..."
cmake --preset Debug

echo
echo "Building project..."
cmake --build --preset Debug

echo
echo "Build complete!"
echo "Generated files in build/Debug/:"
echo "  - blanceCar.elf (ELF executable)"
echo "  - blanceCar.bin (Binary file for flashing)"
echo "  - blanceCar.hex (Intel HEX file)"
echo "  - blanceCar.map (Memory map)"

echo
echo "To flash the binary to your STM32 board:"
echo "  Using OpenOCD:"
echo "    openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg \\"
echo "             -c \"program build/Debug/blanceCar.elf verify reset exit\""
echo
echo "  Using ST-LINK CLI:"
echo "    ST-LINK_CLI -c SWD -P build/Debug/blanceCar.bin 0x08000000 -V -Rst"