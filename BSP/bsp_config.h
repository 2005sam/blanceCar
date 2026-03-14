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

/* Motor driver configuration  */
#define BSP_MOTOR_STBY_GPIO_PORT GPIOA /**< GPIO port for motor standby control */
#define BSP_MOTOR_STBY_PIN GPIO_PIN_1  /**< GPIO pin for motor standby control */

#define BSP_MOTOR_LEFT_IN1_GPIO_PORT GPIOA /**< GPIO port for left motor IN1 */
#define BSP_MOTOR_LEFT_IN1_PIN GPIO_PIN_9  /**< GPIO pin for left motor IN1 */

#define BSP_MOTOR_LEFT_IN2_GPIO_PORT GPIOA /**< GPIO port for left motor IN2 */
#define BSP_MOTOR_LEFT_IN2_PIN GPIO_PIN_10 /**< GPIO pin for left motor IN2 */

#define BSP_MOTOR_LEFT_PWM_TIMER TIM1            /**< Timer for left motor PWM control */
#define BSP_MOTOR_LEFT_PWM_CHANNEL TIM_CHANNEL_1 /**< Timer channel for left motor PWM control */

#define BSP_MOTOR_RIGHT_IN1_GPIO_PORT GPIOB /**< GPIO port for right motor IN1 */
#define BSP_MOTOR_RIGHT_IN1_PIN GPIO_PIN_5  /**< GPIO pin for right motor IN1 */

#define BSP_MOTOR_RIGHT_IN2_GPIO_PORT GPIOB /**< GPIO port for right motor IN2 */
#define BSP_MOTOR_RIGHT_IN2_PIN GPIO_PIN_7  /**< GPIO pin for right motor IN2 */

#define BSP_MOTOR_RIGHT_PWM_TIMER TIM4            /**< Timer for right motor PWM control */
#define BSP_MOTOR_RIGHT_PWM_CHANNEL TIM_CHANNEL_1 /**< Timer channel for right motor PWM control */

#endif /* BSP_CONFIG_H */
