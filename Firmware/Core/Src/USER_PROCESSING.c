#include <USER_PROCESSING.h>
#include <USER_Timer.h>
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

SensorMode currentMode = SENSOR_MODE_BOTH;

volatile uint32_t countPerMinOfGeiger = 0;
volatile uint32_t countPerMinOfBPW34 = 0;

volatile float doseRateOfGeiger = 0;
volatile float doseRateOfBPW34 = 0;
volatile uint8_t msgStateFlag = 0;

float filteredCPM_Geiger = 0;
float filteredCPM_BPW34 = 0;

float cal_a = 1.0f;             
float cal_b = 0.0f; 

void USER_PROCESSING_CPMCallback(void)
{ 
	    countPerMinOfGeiger = countGeiger;
			countGeiger = 0;

      countPerMinOfBPW34 = countBPW34;
			countBPW34 = 0;
}

void USER_PROCESSING_SignalFilter(void){
	
    filteredCPM_Geiger = (1.0f - OPTIMAL_Q) * countPerMinOfGeiger + OPTIMAL_Q * filteredCPM_Geiger;
    countPerMinOfGeiger = (uint32_t)(filteredCPM_Geiger + 0.5f);
	
	  filteredCPM_BPW34 = (1.0f - OPTIMAL_Q) * countPerMinOfBPW34 + OPTIMAL_Q * filteredCPM_BPW34;
    countPerMinOfBPW34 = (uint32_t)(filteredCPM_BPW34 + 0.5f);
}

void USER_PROCESSING_CalculateDoseRate(void){
	
	doseRateOfGeiger = countPerMinOfGeiger * CPM_PER_uSv;
	
	doseRateOfBPW34 = 1.225 * countPerMinOfBPW34 - 273.4;
	
	if(doseRateOfBPW34 < 0) doseRateOfBPW34 = 0;
	
}
void USER_PROCESSING_Calibration(void){
	
	  countPerMinOfGeiger = cal_a * countPerMinOfGeiger + cal_b;
	
		countPerMinOfBPW34 = cal_a * countPerMinOfBPW34 + cal_b;	
}

void USER_PROCESSING_ProcessAndSend(void)
{ 
	  if(periodStateFlag == 1){
			
	  USER_PROCESSING_CPMCallback();
			
		USER_PROCESSING_SignalFilter();
			
		USER_PROCESSING_Calibration();	
			
	  USER_PROCESSING_CalculateDoseRate();
			
	  char message[128];
    
		if (currentMode == SENSOR_MODE_GEIGER_ONLY){
			
			char* current_mode = "GEIGER";
		
			snprintf(message, sizeof(message),
			"{\r\n \"geiger\": %.7f,\r\n \"current_mode\": \"%s\"\r\n}\r\n", doseRateOfGeiger, current_mode);
		
			HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
		
    } else if (currentMode == SENSOR_MODE_BPW34_ONLY) {
		
			char* current_mode = "BP";
			
			snprintf(message, sizeof(message),
			"{\r\n \"bpw34\": %.7f,\r\n \"current_mode\": \"%s\"\r\n}\r\n", doseRateOfBPW34, current_mode);
		
			HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
		 
    } else {
			
				char* current_mode = "BOTH";
			
				snprintf(message, sizeof(message),
				"{\r\n \"bpw34\": %.7f,\r\n \"geiger\": %.7f,\r\n \"current_mode\": \"%s\"\r\n}\r\n", 
				doseRateOfBPW34, doseRateOfGeiger, current_mode);
				
				HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

    }
		
	  countPerMinOfGeiger = 0;
	  countPerMinOfBPW34 = 0;
		
		doseRateOfGeiger = 0;
	  doseRateOfBPW34 = 0;
		
		}
}

void USER_PROCESSING_ProcessUartCommand(char *cmd) {
    char *endptr;

    if (strcmp(cmd, "SENSOR_GEIGER_ONLY") == 0) {
        currentMode = SENSOR_MODE_GEIGER_ONLY;

    } else if (strcmp(cmd, "SENSOR_BPW34_ONLY") == 0) {
        currentMode = SENSOR_MODE_BPW34_ONLY;

    } else if (strcmp(cmd, "SENSOR_GEIGER_BPW34") == 0) {
        currentMode = SENSOR_MODE_BOTH;

    } else if (strncmp(cmd, "SENSOR_FREQ_", 12) == 0) {
        uint32_t val = strtoul(&cmd[12], &endptr, 10);
        if (*endptr == '\0' && val >= 60 && val <= 3600) {
            transPeriod = val;
        } else {
            msgStateFlag = 1;
        }

    } else if (strncmp(cmd, "SENSOR_CALBA_", 13) == 0) {
        float temp = strtof(&cmd[13], &endptr);
        if (*endptr == '\0' && temp > 0) {
            cal_a = temp;
        } else {
            msgStateFlag = 1;
        }

    } else if (strncmp(cmd, "SENSOR_CALBB_", 13) == 0) {
        float temp = strtof(&cmd[13], &endptr);
        if (*endptr == '\0') {
            cal_b = temp;
        } else {
            msgStateFlag = 1;
        }

    } else {
        msgStateFlag = 1;
    }
}
