/**
 * @file    bsp_config.h
 * @author  sam
 * @brief   Board Support Package configuration header
 *
 * This file contains hardware configuration definitions for the balance car project.
 * It defines GPIO pins and ports for various peripherals.
 */

#ifndef BSP_CONFIG_H
#define BSP_CONFIG_H

#include "stm32f1xx_hal.h"

/* LED configuration */
#define BSP_LED_VOLTAGE_1_GPIO_PORT GPIOA /**< GPIO port for voltage level 1 LED */
#define BSP_LED_VOLTAGE_1_PIN GPIO_PIN_4  /**< GPIO pin for voltage level 1 LED */

#define BSP_LED_VOLTAGE_2_GPIO_PORT GPIOA /**< GPIO port for voltage level 2 LED */
#define BSP_LED_VOLTAGE_2_PIN GPIO_PIN_5  /**< GPIO pin for voltage level 2 LED */

#define BSP_LED_VOLTAGE_3_GPIO_PORT GPIOA /**< GPIO port for voltage level 3 LED */
#define BSP_LED_VOLTAGE_3_PIN GPIO_PIN_6  /**< GPIO pin for voltage level 3 LED */

/* Button configuration */
#define BSP_BUTTON_USERKEY_GPIO_PORT GPIOA /**< GPIO port for user key button */
#define BSP_BUTTON_USERKEY_PIN GPIO_PIN_11 /**< GPIO pin for user key button */

#endif /* BSP_CONFIG_H */
