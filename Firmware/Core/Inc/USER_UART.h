#ifndef __USER_UART_H
#define __USER_UART_H

#include "stdint.h"
#include "usart.h"

#define UART_RX_BUFFER_SIZE 64

#define INVALID_MESSAGE "\r\n MESSAGE_INVALID"
#define VALID_MESSAGE "\r\n MESSAGE_VALID"

extern uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
extern uint8_t uartRxIndex;
extern uint8_t uartRxData;
	
            
extern volatile uint32_t uartLastByteTick;
extern volatile uint8_t uartReceiving;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

void UART_StartReceive(void);

void UART_SendInvalidMsg(void);

void UART_SendValidMsg(void);

void UART_ResetBuffer(void);

void UART_TimeOut(void);

#endif

