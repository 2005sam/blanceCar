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

/**
 * @brief  Send data via UART2
 * @param  pData: Pointer to data buffer
 * @param  Size: Number of bytes to send
 * @retval Return code indicating success or error
 */
uint32_t BSP_UART2_Send(uint8_t *pData, uint16_t Size);

#endif /* BSP_UART_H */
