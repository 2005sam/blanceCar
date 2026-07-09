#include "settings.h"
#include "error_codes.h"
#include "bsp_config.h"
#include "bsp_mpu6050.h"
#include "stm32f1xx_hal.h"
#include <math.h>

static volatile bool mpu6050_initialized = false;
static volatile int16_t mpu6050_accel_x = 0;
static volatile int16_t mpu6050_accel_y = 0;
static volatile int16_t mpu6050_accel_z = 0;

extern I2C_HandleTypeDef hi2c1;

static void simple_delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        for (uint32_t j = 0; j < 4000; j++) {
            __NOP();
        }
    }
}

static uint32_t ReadAccelData(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buffer[6];

    if (!mpu6050_initialized) {
        return RET_BSP_HARDWARE;
    }

    if (HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1, buffer, 6, BSP_MPU6050_I2C_TIMEOUT) == HAL_OK) {
        *x = (int16_t)((buffer[0] << 8) | buffer[1]);
        *y = (int16_t)((buffer[2] << 8) | buffer[3]);
        *z = (int16_t)((buffer[4] << 8) | buffer[5]);
        return RET_OK;
    }

    return RET_BSP_HARDWARE;
}

uint32_t BspMpu6050Init(void) {
    uint8_t data;

    if (mpu6050_initialized) {
        return RET_OK;
    }

    simple_delay_ms(100);

    data = MPU6050_PWR1_RESET_BIT;
    if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_PWR_MGMT_1, 1, &data, 1, BSP_MPU6050_I2C_TIMEOUT) != HAL_OK) {
        return RET_BSP_HARDWARE;
    }

    simple_delay_ms(100);

    data = 0x00;
    if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_PWR_MGMT_1, 1, &data, 1, BSP_MPU6050_I2C_TIMEOUT) != HAL_OK) {
        return RET_BSP_HARDWARE;
    }

    simple_delay_ms(10);

    data = BSP_MPU6050_ACCEL_RANGE;
    if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_ACCEL_CONFIG, 1, &data, 1, BSP_MPU6050_I2C_TIMEOUT) != HAL_OK) {
        return RET_BSP_HARDWARE;
    }

    simple_delay_ms(10);

    mpu6050_initialized = true;
    return RET_OK;
}

uint32_t BspMpu6050GetAccelX(int16_t *x) {
    if (x == NULL) {
        return RET_BSP_INVALID_PARAM;
    }
    int16_t y, z;
    uint32_t ret = ReadAccelData(x, &y, &z);
    if (ret != RET_OK) {
        return ret;
    }
    return RET_OK;
}

uint32_t BspMpu6050GetAccelY(int16_t *y) {
    if (y == NULL) {
        return RET_BSP_INVALID_PARAM;
    }
    int16_t x, z;
    uint32_t ret = ReadAccelData(&x, y, &z);
    if (ret != RET_OK) {
        return ret;
    }
    return RET_OK;
}

uint32_t BspMpu6050GetAccelZ(int16_t *z) {
    if (z == NULL) {
        return RET_BSP_INVALID_PARAM;
    }
    int16_t x, y;
    uint32_t ret = ReadAccelData(&x, &y, z);
    if (ret != RET_OK) {
        return ret;
    }
    return RET_OK;
}

uint32_t BspMpu6050GetPitch(float *pitch) {
    if (pitch == NULL) {
        return RET_BSP_INVALID_PARAM;
    }
    int16_t x, y, z;
    uint32_t ret = ReadAccelData(&x, &y, &z);
    if (ret != RET_OK) {
        return ret;
    }
    float accel_x = (float)x / BSP_MPU6050_ACCEL_SCALE;
    float accel_y = (float)y / BSP_MPU6050_ACCEL_SCALE;
    float accel_z = (float)z / BSP_MPU6050_ACCEL_SCALE;
    *pitch = atan2(accel_y, sqrt(accel_x * accel_x + accel_z * accel_z)) * (180.0f / M_PI);
    return RET_OK;
}

uint32_t BspMpu6050GetRoll(float *roll) {
    if (roll == NULL) {
        return RET_BSP_INVALID_PARAM;
    }
    int16_t x, y, z;
    uint32_t ret = ReadAccelData(&x, &y, &z);
    if (ret != RET_OK) {
        return ret;
    }
    float accel_x = (float)x / BSP_MPU6050_ACCEL_SCALE;
    float accel_y = (float)y / BSP_MPU6050_ACCEL_SCALE;
    float accel_z = (float)z / BSP_MPU6050_ACCEL_SCALE;
    *roll = atan2(-accel_x, sqrt(accel_y * accel_y + accel_z * accel_z)) * (180.0f / M_PI);
    return RET_OK;
}

uint32_t BspMpu6050GetYaw(float *yaw) {
    if (yaw == NULL) {
        return RET_BSP_INVALID_PARAM;
    }
    *yaw = 0.0f;
    return RET_OK;
}
