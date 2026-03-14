/**
 * @file    bsp_led.c
 * @author  sam
 * @brief   Board Support Package LED driver implementation
 *
 * This file contains the LED control functions for the balance car project.
 * It provides functions to turn LEDs on and off based on LED identifiers.
 */

#include "bsp_led.h"
#include "bsp_uart.h"
#include "stm32f1xx_hal.h"
#include "settings.h"
#include <stdint.h>
#include "bsp_config.h"

/**
 * @brief  Turn on the specified LED
 * @param  led: LED identifier ( VOLTAGE_1, VOLTAGE_2, VOLTAGE_3)
 * @retval Return code indicating success or error
 */
uint32_t LedLightOn(LedId led)
{
  switch (led)
  {
  case VOLTAGE_1:
    HAL_GPIO_WritePin(BSP_LED_VOLTAGE_1_GPIO_PORT, BSP_LED_VOLTAGE_1_PIN, GPIO_PIN_SET);
    break;
  case VOLTAGE_2:
    HAL_GPIO_WritePin(BSP_LED_VOLTAGE_2_GPIO_PORT, BSP_LED_VOLTAGE_2_PIN, GPIO_PIN_SET);
    break;
  case VOLTAGE_3:
    HAL_GPIO_WritePin(BSP_LED_VOLTAGE_3_GPIO_PORT, BSP_LED_VOLTAGE_3_PIN, GPIO_PIN_SET);
    break;
  default:
    return MAKE_RETURN_CODE(ERROR, ERROR_LEVEL_MAJOR, MODULE_LED, ERR_TYPE_INVALID_PARAM, 0x01);
  }
  return MAKE_RETURN_CODE(SUCCESS, ERROR_LEVEL_MINOR, MODULE_LED, ERR_TYPE_SUCCESS, 0x00);
}

/**
 * @brief  Turn off the specified LED
 * @param  led: LED identifier ( VOLTAGE_1, VOLTAGE_2, VOLTAGE_3)
 * @retval Return code indicating success or error
 */
uint32_t LedLightOff(LedId led)
{
  switch (led)
  {
  case VOLTAGE_1:
    HAL_GPIO_WritePin(BSP_LED_VOLTAGE_1_GPIO_PORT, BSP_LED_VOLTAGE_1_PIN, GPIO_PIN_RESET);
    break;
  case VOLTAGE_2:
    HAL_GPIO_WritePin(BSP_LED_VOLTAGE_2_GPIO_PORT, BSP_LED_VOLTAGE_2_PIN, GPIO_PIN_RESET);
    break;
  case VOLTAGE_3:
    HAL_GPIO_WritePin(BSP_LED_VOLTAGE_3_GPIO_PORT, BSP_LED_VOLTAGE_3_PIN, GPIO_PIN_RESET);
    break;
  default:
    return MAKE_RETURN_CODE(ERROR, ERROR_LEVEL_MAJOR, MODULE_LED, ERR_TYPE_INVALID_PARAM, 0x01);
  }
  return MAKE_RETURN_CODE(SUCCESS, ERROR_LEVEL_MINOR, MODULE_LED, ERR_TYPE_SUCCESS, 0x00);
}
