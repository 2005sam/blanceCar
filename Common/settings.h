#ifndef SETTINGS_H
#define SETTINGS_H

// return code format: [success(1 bit)][error level(2 bits)][module(5 bits)][class(8 bits)][code(16 bits)]
#define RETURN_SUCCESS_SHIFT 31
#define RETURN_ERROR_LEVEL_SHIFT 29
#define RETURN_MODULE_SHIFT 24
#define RETURN_CLASS_SHIFT 16
#define RETURN_CODE_SHIFT 0
#define MAKE_RETURN_CODE(success, error_level, module, class, code) \
  (((success) << RETURN_SUCCESS_SHIFT) |                            \
   ((error_level) << RETURN_ERROR_LEVEL_SHIFT) |                    \
   ((module) << RETURN_MODULE_SHIFT) |                              \
   ((class) << RETURN_CLASS_SHIFT) |                                \
   ((code) << RETURN_CODE_SHIFT))

// Success and error codes
typedef enum
{
  RETURN_SUCCESS = 0,
  RETURN_ERROR = 1
} ReturnStatus;

// Error levels
typedef enum
{
  ERROR_LEVEL_MINOR = 0,
  ERROR_LEVEL_MEDIUM = 1,
  ERROR_LEVEL_MAJOR = 2,
  ERROR_LEVEL_CRITICAL = 3
} ErrorLevel;

// Modules ID
typedef enum
{
  MODULE_NONE = 0x00,
  MODULE_OSAL = 0x01,
  MODULE_HAL = 0x02,
  MODULE_UART = 0x03,
  MODULE_LED = 0x04,
} ModuleID;

// Class
typedef enum
{
  // Success (placeholder, not used in error codes)
  ERR_TYPE_SUCCESS = 0x00,

  // Initialization related (0x01-0x0F)
  ERR_TYPE_INIT_FAIL = 0x01,       // Initialization failed
  ERR_TYPE_INIT_TIMEOUT = 0x02,    // Initialization timeout
  ERR_TYPE_INIT_NOT_CALLED = 0x03, // Called without prior initialization

  // Parameter related (0x10-0x1F)
  ERR_TYPE_INVALID_PARAM = 0x10, // Invalid parameter (out of range)
  ERR_TYPE_NULL_POINTER = 0x11,  // Null pointer
  ERR_TYPE_MISALIGNED = 0x12,    // Address misalignment

  // Resource related (0x20-0x2F)
  ERR_TYPE_BUSY = 0x20,          // Resource busy
  ERR_TYPE_TIMEOUT = 0x21,       // Operation timeout
  ERR_TYPE_NO_RESOURCE = 0x22,   // No available resource (e.g., queue full)
  ERR_TYPE_OUT_OF_MEMORY = 0x23, // Out of memory

  // Data related (0x30-0x3F)
  ERR_TYPE_NO_DATA = 0x30,      // No data available
  ERR_TYPE_DATA_CORRUPT = 0x31, // Data corrupted
  ERR_TYPE_OVERFLOW = 0x32,     // Buffer overflow
  ERR_TYPE_UNDERFLOW = 0x33,    // Buffer underflow

  // Hardware related (0x40-0x4F)
  ERR_TYPE_HARDWARE = 0x40,         // Generic hardware failure
  ERR_TYPE_COMM_FAIL = 0x41,        // Communication failure
  ERR_TYPE_SENSOR_FAIL = 0x42,      // Sensor failure
  ERR_TYPE_DEVICE_NOT_FOUND = 0x43, // Device not responding

  // State/Logic related (0x50-0x5F)
  ERR_TYPE_INVALID_STATE = 0x50,    // Invalid state
  ERR_TYPE_OPERATION_DENIED = 0x51, // Operation not allowed
  ERR_TYPE_NOT_SUPPORTED = 0x52,    // Feature not supported

  // Calibration/Configuration related (0x60-0x6F)
  ERR_TYPE_CALIB_REQUIRED = 0x60, // Calibration required
  ERR_TYPE_CONFIG_ERROR = 0x61,   // Configuration error

  // Application-specific (0x70-0xEF) for project-defined types
  // Reserved (0xF0-0xFF) for future expansion
} ErrorType;
#endif
