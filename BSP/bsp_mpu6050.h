#ifndef BSP_MPU6050_H
#define BSP_MPU6050_H

#include <stdint.h>
#include <stdbool.h>

/* MPU6050 Register Definitions */
#define MPU6050_ADDR_7BIT   0x68    /**< MPU6050 7-bit I2C address */
#define MPU6050_ADDR        (0x68 << 1)  /**< MPU6050 I2C address (left-shifted for STM32 HAL) */
#define MPU6050_PWR_MGMT_1  0x6B    /**< Power management 1 register */
#define MPU6050_ACCEL_CONFIG 0x1C   /**< Accelerometer configuration register */
#define MPU6050_ACCEL_XOUT_H 0x3B   /**< Accelerometer X-axis high byte */
#define MPU6050_ACCEL_XOUT_L 0x3C   /**< Accelerometer X-axis low byte */
#define MPU6050_ACCEL_YOUT_H 0x3D   /**< Accelerometer Y-axis high byte */
#define MPU6050_ACCEL_YOUT_L 0x3E   /**< Accelerometer Y-axis low byte */
#define MPU6050_ACCEL_ZOUT_H 0x3F   /**< Accelerometer Z-axis high byte */
#define MPU6050_ACCEL_ZOUT_L 0x40   /**< Accelerometer Z-axis low byte */

/* MPU6050 Power Management Bits */
#define MPU6050_PWR1_RESET_BIT    0x80  /**< Device reset bit */
#define MPU6050_PWR1_SLEEP_BIT    0x40  /**< Sleep mode bit */

/* Sensor Data Structure */
typedef struct {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    float pitch;
    float roll;
} Mpu6050Data_t;

/* Public API Functions */

/**
 * @brief Initialize MPU6050 sensor
 * @return RET_OK on success, RET_BSP_INVALID_PARAM or RET_BSP_HARDWARE on failure
 */
uint32_t BspMpu6050Init(void);

/**
 * @brief Get X-axis accelerometer value
 * @param x Pointer to store X-axis acceleration (LSB)
 * @return RET_OK on success, RET_BSP_INVALID_PARAM if x is NULL, RET_BSP_HARDWARE on I2C failure
 */
uint32_t BspMpu6050GetAccelX(int16_t *x);

/**
 * @brief Get Y-axis accelerometer value
 * @param y Pointer to store Y-axis acceleration (LSB)
 * @return RET_OK on success, RET_BSP_INVALID_PARAM if y is NULL, RET_BSP_HARDWARE on I2C failure
 */
uint32_t BspMpu6050GetAccelY(int16_t *y);

/**
 * @brief Get Z-axis accelerometer value
 * @param z Pointer to store Z-axis acceleration (LSB)
 * @return RET_OK on success, RET_BSP_INVALID_PARAM if z is NULL, RET_BSP_HARDWARE on I2C failure
 */
uint32_t BspMpu6050GetAccelZ(int16_t *z);

/**
 * @brief Get pitch angle (rotation around X-axis)
 * @param pitch Pointer to store pitch angle in degrees (-90 to +90)
 * @return RET_OK on success, RET_BSP_INVALID_PARAM if pitch is NULL, RET_BSP_HARDWARE on I2C failure
 */
uint32_t BspMpu6050GetPitch(float *pitch);

/**
 * @brief Get roll angle (rotation around Y-axis)
 * @param roll Pointer to store roll angle in degrees (-90 to +90)
 * @return RET_OK on success, RET_BSP_INVALID_PARAM if roll is NULL, RET_BSP_HARDWARE on I2C failure
 */
uint32_t BspMpu6050GetRoll(float *roll);

/**
 * @brief Get yaw angle (rotation around Z-axis)
 * @param yaw Pointer to store yaw angle in degrees
 * @return RET_OK on success, RET_BSP_INVALID_PARAM if yaw is NULL
 * @note Yaw cannot be determined from accelerometer alone; returns 0.
 *       Requires gyroscope integration or magnetometer for actual yaw.
 */
uint32_t BspMpu6050GetYaw(float *yaw);

#endif