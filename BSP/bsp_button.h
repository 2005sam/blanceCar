#ifndef BSP_BUTTON_H_
#define BSP_BUTTON_H_

#include "stm32f1xx_hal.h"

typedef enum
{
  BSP_BUTTON_ON = 0,
  BSP_BUTTON_OFF = 1
} UserButtonState;

uint32_t BspUserButtonHandeler(void);
uint32_t BspUserButtonGetState(UserButtonState *state);

#endif /* BSP_BUTTON_H_ */
