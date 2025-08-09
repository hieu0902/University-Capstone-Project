#include <USER_UART.h>
#include <USER_PROCESSING.h>
#include "string.h"


uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t uartRxIndex = 0;
uint8_t uartRxData;
	
volatile uint32_t uartLastByteTick = 0;
volatile uint8_t uartReceiving = 0;

void UART_ResetBuffer(void) {
    memset(uartRxBuffer, 0, UART_RX_BUFFER_SIZE);  
    uartRxIndex = 0;                               
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	      uartLastByteTick = HAL_GetTick();   
        uartReceiving = 1;
		 if (huart->Instance == USART2) {
			 
        if (uartRxData == '\\') {
					
            uartRxBuffer[uartRxIndex] = '\0'; 
            USER_PROCESSING_ProcessUartCommand((char *)uartRxBuffer);
					
						if(msgStateFlag == 1){
							
					  msgStateFlag = 0;
			
					  UART_SendInvalidMsg();
							
				    } else {
							
					  UART_SendValidMsg();
							
				    }
						
						UART_ResetBuffer();
						
            uartRxIndex = 0;
						
					  uartReceiving = 0;
        } else {
            if (uartRxIndex < UART_RX_BUFFER_SIZE - 1) {
							
                uartRxBuffer[uartRxIndex++] = uartRxData;
							
            } else {
							
							UART_ResetBuffer();  
              msgStateFlag = 1;
							
              uartReceiving = 0;
							uartRxIndex = 0;
							
            }
        }
				HAL_UART_Receive_IT(&huart2, &uartRxData, 1);
    }
		
}
void UART_TimeOut(void){	
	if (uartReceiving && (HAL_GetTick() - uartLastByteTick > 300)) {
    UART_ResetBuffer();
    UART_SendInvalidMsg();
    uartReceiving = 0;
  }
}
void UART_StartReceive(void) {
	  
  HAL_UART_Receive_IT(&huart2, &uartRxData, 1);
	
}
void UART_SendInvalidMsg(void){
	
	HAL_UART_Transmit(&huart2, (uint8_t*)INVALID_MESSAGE, strlen(INVALID_MESSAGE), HAL_MAX_DELAY);
}
void UART_SendValidMsg(void){
	
	HAL_UART_Transmit(&huart2, (uint8_t*)VALID_MESSAGE, strlen(VALID_MESSAGE), HAL_MAX_DELAY);
	
}

