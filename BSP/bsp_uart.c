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
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

extern UART_HandleTypeDef huart2;

/**
 * @brief  Send data via UART2
 * @param  pData: Pointer to data buffer
 * @param  Size: Number of bytes to send
 * @retval Return code indicating success or error
 */
uint32_t BspUart2Send(uint8_t *pData, uint16_t Size)
{
  if (HAL_UART_Transmit(&huart2, pData, Size, HAL_MAX_DELAY) != HAL_OK)
  {
    return MAKE_RETURN_CODE(ERROR, ERROR_LEVEL_MAJOR, MODULE_UART, ERR_TYPE_INIT_FAIL, 0x01);
  }
  return MAKE_RETURN_CODE(SUCCESS, 0, MODULE_UART, ERR_TYPE_SUCCESS, 0);
}

uint32_t BspUart2Printf(const char *format, ...)
{
  uint32_t ret = 0;
  char buffer[128];
  va_list args;
  va_start(args, format);
  int len = vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  if (len > 0)
  {
    ret = BspUart2Send((uint8_t *)buffer, len);
  }
  else
  {
    ret = MAKE_RETURN_CODE(ERROR, ERROR_LEVEL_MINOR, MODULE_UART, ERR_TYPE_INVALID_PARAM, 0x02);
  }
  return ret;
}