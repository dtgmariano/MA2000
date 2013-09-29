/*
 * uart.h
 *
 *  Created on: 12/02/2011
 *      Author: FELIPE
 */


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "uart.h"
/*
#include "FreeRTOS.h"
#include "task.h"
*/

void vUART_ISR_Wrapper( void ) __attribute__((interrupt("IRQ"),naked));
void UART_Init(uint8_t channel, uint16_t baud_rate);
void UART_SendBuffer(uint8_t channel, const uint8_t *Buffer, uint16_t size);
uint16_t UART_ReceiveBuffer(uint8_t channel, uint8_t *Buffer, uint16_t size);
void UART_putc(char c);
int UART_GetNumberAvailableElements(void);

#endif /* UART_H_ */

