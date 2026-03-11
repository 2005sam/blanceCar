#ifndef __BSP_BUTTON_H
#define __BSP_BUTTON_H

#include "stm32f1xx_hal.h"
typedef enum
{
  ON = 0,
  OFF = 1
} USER_BUTTON_STATE;
uint32_t BspUserButtonHandeler(void);
uint32_t BspUserButtonGetState(USER_BUTTON_STATE *state);

#endif