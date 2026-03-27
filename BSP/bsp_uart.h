/**
 * @file    bsp_uart.h
 * @author  sam
 * @brief   Board Support Package UART driver header
 *
 * This file contains the UART driver interface definitions for the balance car project.
 */

#ifndef BSP_UART_H
#define BSP_UART_H
#include <stdint.h>

#define RX_BUFFER_SIZE 10

uint32_t BspUart2Init(void);
uint32_t BspUart2Send(uint8_t *pData, uint16_t Size);
uint32_t BspUart2Printf(const char *format, ...);
uint32_t BspUart2Read(uint8_t *pData);
#endif /* BSP_UART_H */
