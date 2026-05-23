# blanceCar - STM32F103 Balancing Car Project

This is an STM32F103-based balancing car project using the STM32 HAL library.

## Project Structure

```
.
├── Application/            # Application layer (task definitions, control logic)
├── BSP/                    # Board Support Package (hardware drivers)
│   ├── bsp_config.h        # Hardware pin mapping configuration
│   ├── bsp_led.c/h         # LED driver
│   ├── bsp_button.c/h      # Button driver
│   ├── bsp_motor.c/h       # Motor driver (PWM + direction control)
│   └── bsp_uart.c/h        # UART communication driver
├── Common/                 # Shared project definitions
│   └── settings.h          # Return code system and error types
├── Core/                   # STM32CubeMX generated core files
│   ├── Inc/                # HAL configuration headers
│   └── Src/                # HAL initialization and interrupt handlers
├── Drivers/                # STM32 HAL drivers and CMSIS
├── OSAL/                   # Operating System Abstraction Layer (optional)
│   ├── osal.h              # Portable OSAL interface
│   └── src/osal_freertos.c # FreeRTOS implementation (not yet integrated)
├── cmake/                  # CMake toolchain and STM32CubeMX integration
├── docs/                   # Project documentation
│   └── ARCHITECTURE.md     # Architecture documentation
├── startup_stm32f103xb.s   # Startup assembly
├── STM32F103XX_FLASH.ld    # Linker script
├── blanceCar.ioc           # STM32CubeMX project file
├── CMakeLists.txt          # Main CMake configuration
├── CMakePresets.json       # CMake presets for Debug/Release builds
└── .mxproject              # STM32CubeMX metadata
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

### MCU and Clock

| Parameter        | Value                      |
| ---------------- | -------------------------- |
| **MCU**          | STM32F103xB (Cortex-M3)    |
| **Flash**        | 128KB                      |
| **RAM**          | 20KB                       |
| **Oscillator**   | HSE 8MHz external crystal  |
| **System Clock** | 72MHz (HSE ×9 PLL)         |
| **APB1**         | 36MHz (HCLK/2)             |
| **APB2**         | 72MHz (HCLK/1)             |
| **Toolchain**    | GNU Arm Embedded Toolchain |
| **Build System** | CMake + Ninja              |

### Peripheral Configuration

| Peripheral | Function               | Pin(s)             | Details                 |
| ---------- | ---------------------- | ------------------ | ----------------------- |
| **USART2** | Debug/Bluetooth UART   | PA2 (TX), PA3 (RX) | 115200-8N1              |
| **TIM1**   | Left motor PWM         | PA8 (CH1)          | PWM, Period = 65535     |
| **TIM4**   | Right motor PWM        | PB6 (CH1)          | PWM, Period = 65535     |
| **TIM2**   | HAL timebase (SysTick) | -                  | Period elapsed callback |

### GPIO Pin Mapping

| Pin  | Function             | Configuration         |
| ---- | -------------------- | --------------------- |
| PA1  | Motor STBY           | Output Push-Pull      |
| PA4  | LED Voltage Level 1  | Output Push-Pull      |
| PA5  | LED Voltage Level 2  | Output Push-Pull      |
| PA6  | LED Voltage Level 3  | Output Push-Pull      |
| PA8  | TIM1_CH1 (Left PWM)  | AF Push-Pull          |
| PA9  | Left Motor IN1       | Output Push-Pull      |
| PA10 | Left Motor IN2       | Output Push-Pull      |
| PA11 | User Key Button      | Input, Rising Edge IT |
| PB5  | Right Motor IN1      | Output Push-Pull      |
| PB6  | TIM4_CH1 (Right PWM) | AF Push-Pull          |
| PB7  | Right Motor IN2      | Output Push-Pull      |

### Interrupt Configuration

| Interrupt      | Priority | Source             |
| -------------- | -------- | ------------------ |
| TIM2_IRQn      | 0        | HAL timebase tick  |
| USART2_IRQn    | 0        | UART communication |
| EXTI15_10_IRQn | 0        | PA11 button press  |

### OSAL (Operating System Abstraction Layer)

The OSAL provides a portable RTOS abstraction interface. Currently:

- **osal.h**: Full API definition (tasks, semaphores, mutexes, queues, timers, event groups)
- **osal_freertos.c**: FreeRTOS implementation (available but **not yet integrated** into the build)
- To enable: uncomment the OSAL lines in `CMakeLists.txt` and ensure FreeRTOS sources are available

## Return Code System

The project uses a standardized 32-bit return code system defined in `Common/settings.h`:

```
[success(1 bit)][error level(2 bits)][module(5 bits)][class(8 bits)][code(16 bits)]
```

- **Modules**: OSAL, HAL, UART, LED, Button, Motor
- **Error Classes**: Initialization, Parameter, Resource, Data, Hardware, State, Calibration

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Next Steps

1. Install CMake from https://cmake.org/download/
2. Run `cmake --preset Debug` to configure the build
3. Run `cmake --build --preset Debug` to build the project
4. Flash the generated binary to your STM32 board
5. Start developing your balancing car application!
