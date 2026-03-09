#include "bsp_uart.h"
#include "stm32f1xx_hal.h"
#include "settings.h"
#include <stdint.h>
extern UART_HandleTypeDef huart2;
uint32_t BSP_UART2_Send(uint8_t *pData, uint16_t Size)
{
  if (HAL_UART_Transmit(&huart2, pData, Size, HAL_MAX_DELAY) != HAL_OK)
  {
    return MAKE_RETURN_CODE(ERROR, ERROR_LEVEL_MAJOR, MODULE_UART, ERR_TYPE_INIT_FAIL, 0x01);
  }
  return MAKE_RETURN_CODE(SUCCESS, 0, MODULE_UART, ERR_TYPE_SUCCESS, 0);
}
