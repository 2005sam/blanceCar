# blanceCar - STM32F103 Balancing Car Project

This is an STM32F103-based balancing car project using FreeRTOS and the STM32 HAL library.

## Project Structure

```
.
├── Core/                  # Application core files (main.c, HAL configurations)
├── Drivers/              # STM32 HAL drivers and CMSIS
├── Middlewares/          # FreeRTOS middleware
├── cmake/                # CMake toolchain and STM32CubeMX integration
├── startup_stm32f103xb.s # Startup assembly
├── STM32F103XX_FLASH.ld  # Linker script
├── blanceCar.ioc         # STM32CubeMX project file
├── CMakeLists.txt        # Main CMake configuration
├── CMakePresets.json     # CMake presets for Debug/Release builds
└── .mxproject            # STM32CubeMX metadata
```

## Prerequisites

1. **ARM GCC Toolchain** - `arm-none-eabi-gcc`
   - Already installed on this system (version 13.2.1)
   - Download from: [Arm GNU Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)

2. **CMake** (version 3.22 or higher)
   - Not currently installed on this system
   - Download from: [CMake Download](https://cmake.org/download/)
   - Or install via package manager:
     - Windows: `choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'`
     - Or: `scoop install cmake`

3. **Ninja Build System** (optional but recommended)
   - Already installed on this system (version 1.13.1)
   - CMake presets are configured to use Ninja

## Building the Project

### Using CMake Presets (Recommended)

```bash
# Configure and build Debug version
cmake --preset Debug
cmake --build --preset Debug

# Configure and build Release version
cmake --preset Release
cmake --build --preset Release
```

### Manual Build

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake

# Build
ninja
```

### Build Output

The build will produce:

- `blanceCar.elf` - ELF executable
- `blanceCar.map` - Memory map file
- `blanceCar.bin` - Binary file (can be flashed to device)
- `blanceCar.hex` - Intel HEX file

## Flashing to Device

### Using OpenOCD

```bash
openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "program blanceCar.elf verify reset exit"
```

### Using ST-LINK CLI

```bash
ST-LINK_CLI -c SWD -P blanceCar.bin 0x08000000 -V -Rst
```

## Development Setup

### Visual Studio Code

1. Install the following extensions:
   - **C/C++** (Microsoft)
   - **CMake Tools** (Microsoft)
   - **Cortex-Debug** (marus25)

2. Open the project folder in VS Code
3. The CMake Tools extension should automatically detect the CMake project
4. Select the `arm-none-eabi-gcc` kit when prompted
5. Use the build/debug buttons in the status bar

### Git

The project is already initialized as a git repository. To track all project files:

```bash
git add .
git commit -m "Initial project files"
```

## Project Configuration

- **MCU**: STM32F103xB (Cortex-M3, 128KB Flash, 20KB RAM)
- **Clock**: 8MHz HSI internal oscillator
- **RTOS**: FreeRTOS with CMSIS-RTOS v2 wrapper
- **Toolchain**: GNU Arm Embedded Toolchain
- **Build System**: CMake + Ninja

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Next Steps

1. Install CMake from https://cmake.org/download/
2. Run `cmake --preset Debug` to configure the build
3. Run `cmake --build --preset Debug` to build the project
4. Flash the generated binary to your STM32 board
5. Start developing your balancing car application!
