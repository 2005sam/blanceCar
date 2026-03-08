# OSAL (Operating System Abstraction Layer)

## Overview

The OSAL (Operating System Abstraction Layer) provides a portable interface for operating system services, allowing application code to be written once and run on different RTOS implementations. This implementation includes support for FreeRTOS.

## Features

- **Task Management**: Create, delete, delay, and manage tasks
- **Synchronization**: Binary and counting semaphores, mutexes
- **Inter-task Communication**: Message queues
- **Timers**: One-shot and periodic timers with callback support
- **Event Groups**: Bit-based event synchronization
- **System Utilities**: Tick counting, time conversion

## Architecture

### File Structure

```
OSAL/
├── osal.h              # Portable OSAL interface (platform-independent)
├── README.md           # This documentation
└── src/
    └── osal_freertos.c # FreeRTOS-specific implementation
```

### Design Principles

1. **Portability**: The interface (`osal.h`) is designed to be implemented on any RTOS
2. **Type Safety**: Opaque handles prevent misuse of OS objects
3. **Error Handling**: Consistent return codes using the project's error system
4. **Memory Management**: Proper cleanup of allocated resources

## API Usage

### Task Creation

```c
#include "osal.h"

void my_task(void *param) {
    while (1) {
        // Task code
        osal_task_delay(1000); // Delay for 1 second
    }
}

int main(void) {
    osal_task_handle_t task_handle;
    uint32_t result = osal_task_create(
        "MyTask",
        my_task,
        NULL,
        1024,  // Stack size in bytes
        OSAL_PRIORITY_NORMAL,
        &task_handle,
        0
    );

    if (result == SUCCESS) {
        // Task created successfully
    }
}
```

### Semaphore Usage

```c
osal_semaphore_handle_t sem_handle;
uint32_t result = osal_semaphore_create(
    OSAL_SEMAPHORE_BINARY,
    1,  // Max count
    0,  // Initial count
    &sem_handle
);

// Wait for semaphore (blocking)
result = osal_semaphore_take(sem_handle, OSAL_WAIT_FOREVER);

// Release semaphore
result = osal_semaphore_give(sem_handle);
```

### Timer Usage

```c
void timer_callback(void *param) {
    // Timer expired
}

osal_timer_handle_t timer_handle;
uint32_t result = osal_timer_create(
    "MyTimer",
    timer_callback,
    NULL,
    1000,  // 1 second period
    OSAL_TIMER_PERIODIC,
    true,  // Auto-start
    &timer_handle
);
```

## Return Codes

The OSAL uses the project's standardized return code system defined in `Common/settings.h`. Each function returns a 32-bit value with the following format:

```
[success(1 bit)][error level(2 bits)][module(5 bits)][class(8 bits)][code(16 bits)]
```

- **Module ID**: `MODULE_OSAL` (0x01) for OSAL functions
- **Error Types**: Defined in `settings.h` (e.g., `ERR_TYPE_NULL_POINTER`, `ERR_TYPE_TIMEOUT`)

## FreeRTOS Implementation Details

### Priority Mapping

OSAL priorities are mapped to FreeRTOS priorities as follows:

| OSAL Priority | FreeRTOS Priority        |
| ------------- | ------------------------ |
| IDLE          | tskIDLE_PRIORITY         |
| LOW           | tskIDLE_PRIORITY + 1     |
| BELOW_NORMAL  | tskIDLE_PRIORITY + 2     |
| NORMAL        | tskIDLE_PRIORITY + 3     |
| ABOVE_NORMAL  | tskIDLE_PRIORITY + 4     |
| HIGH          | tskIDLE_PRIORITY + 5     |
| REALTIME      | configMAX_PRIORITIES - 1 |

### Memory Management

- **Dynamic Allocation**: Uses FreeRTOS memory management (`pvPortMalloc`/`vPortFree`)
- **Timer Callbacks**: Special structure allocated to store both callback and parameter
- **Cleanup**: All resources are properly freed when objects are deleted

### Time Handling

- **Milliseconds to Ticks**: Uses `pdMS_TO_TICKS()` macro
- **Infinite Wait**: `OSAL_WAIT_FOREVER` maps to `portMAX_DELAY`
- **Tick Counting**: Uses `xTaskGetTickCount()` for system tick count

## Integration with Build System

The OSAL is integrated into the CMake build system:

1. **Source File**: `OSAL/src/osal_freertos.c` added to target sources
2. **Include Directories**: `OSAL` and `Common` directories added to include paths
3. **Dependencies**: Requires FreeRTOS headers and the project's `settings.h`

## Adding Support for Another RTOS

To support another RTOS, create a new implementation file (e.g., `osal_rtx.c`) with the same API defined in `osal.h`. The implementation should:

1. Include `osal.h` and the target RTOS headers
2. Implement all functions declared in `osal.h`
3. Map OSAL APIs to the target RTOS primitives
4. Use the same return code format

## Testing

Basic compilation test:

```bash
# From project root
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

## License

This OSAL implementation is part of the blanceCar project and follows the project's licensing terms.

## References

- [FreeRTOS Documentation](https://www.freertos.org/)
- [CMSIS-RTOS v2 API](https://arm-software.github.io/CMSIS_5/RTOS2/html/index.html)
- Project-specific error codes in `Common/settings.h`
