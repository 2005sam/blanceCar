/**
 * @file    bsp_uart.c
 * @author  sam
 * @brief   Board Support Package UART driver implementation
 *
 * This file contains the UART communication functions for the balance car project.
 * It provides functions for sending data via UART2.
 */

#include "bsp_uart.h"
#include "stm32f1xx_hal.h"
#include "settings.h"
#include <stdint.h>

extern UART_HandleTypeDef huart2;

/**
 * @brief  Send data via UART2
 * @param  pData: Pointer to data buffer
 * @param  Size: Number of bytes to send
 * @retval Return code indicating success or error
 */
uint32_t BSP_UART2_Send(uint8_t *pData, uint16_t Size)
{
  if (HAL_UART_Transmit(&huart2, pData, Size, HAL_MAX_DELAY) != HAL_OK)
  {
    return MAKE_RETURN_CODE(ERROR, ERROR_LEVEL_MAJOR, MODULE_UART, ERR_TYPE_INIT_FAIL, 0x01);
  }
  return MAKE_RETURN_CODE(SUCCESS, 0, MODULE_UART, ERR_TYPE_SUCCESS, 0);
}
