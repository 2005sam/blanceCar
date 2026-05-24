#ifndef BSP_BATTERY_H_
#define BSP_BATTERY_H_
#include "stm32f1xx_hal.h"

uint32_t BspBatteryInit(void);
uint32_t BspBatteryGetVoltage(float *voltage);

#endif