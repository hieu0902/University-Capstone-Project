#ifndef __USER_PROCESSING_H
#define __USER_PROCESSING_H


#include "stdint.h"


#define CPM_PER_uSv   0.00332f
#define OPTIMAL_Q     0.5f

typedef enum {
    SENSOR_MODE_GEIGER_ONLY,
    SENSOR_MODE_BPW34_ONLY,
    SENSOR_MODE_BOTH
} SensorMode;
 
extern SensorMode currentMode;
extern volatile uint32_t countPerMinOfGeiger;
extern volatile uint32_t countPerMinOfBPW34;

extern volatile float doseRateOfGeiger;
extern volatile float doseRateOfBPW34;
extern volatile uint8_t msgStateFlag;

extern float filteredCPM_Geiger;
extern float filteredCPM_BPW34;

extern float cal_a;             
extern float cal_b; 

extern char* current_mode;
 
void USER_PROCESSING_ProcessAndSend(void);

void USER_PROCESSING_CPMCallback(void);

void USER_PROCESSING_SignalFilter(void);

void USER_PROCESSING_CalculateDoseRate(void);

void USER_PROCESSING_ProcessUartCommand(char *cmd);

void USER_PROCESSING_Calibration(void);

#endif

