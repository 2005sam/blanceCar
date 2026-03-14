/**
 * @file    bsp_led.h
 * @author  sam
 * @brief   Board Support Package LED driver header
 *
 * This file contains the LED driver interface definitions for the balance car project.
 * It defines LED identifiers and function prototypes for LED control.
 */

#ifndef BSP_LED_H
#define BSP_LED_H

#include <stdint.h>

/**
 * @brief  LED identifiers
 */
typedef enum
{
  VOLTAGE_1 = 2, /**< Voltage level 1 indicator LED */
  VOLTAGE_2 = 3, /**< Voltage level 2 indicator LED */
  VOLTAGE_3 = 4, /**< Voltage level 3 indicator LED */
} LedId;

/**
 * @brief  Turn on the specified LED
 * @param  led: LED identifier(choosed from LED_ID)
 * @retval Return code indicating success or error
 */
uint32_t LedLightOn(LedId led);

/**
 * @param  led: LED identifier(choosed from LED_ID)
 * @brief  Turn off the specified LED
 * @retval Return code indicating success or error
 */
uint32_t LedLightOff(LedId led);

#endif /* BSP_LED_H */
