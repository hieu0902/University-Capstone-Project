#ifndef __USER_TIMER_H
#define __USER_TIMER_H


#include "stdint.h"
#include "gpio.h"

#define TIME_GAP_MIN_US 10000 
#define USV_THRESHOLD 1.0f

extern volatile uint32_t lastCaptureTick;
extern volatile uint32_t countGeiger;

extern volatile uint32_t lastTickBPW34;
extern volatile uint32_t countBPW34;

extern volatile uint8_t periodStateFlag;
extern volatile uint32_t periodCounter;
extern volatile uint32_t transPeriod;


void USER_TIMER_StartPWM(void);

void USER_TIMER_StartUserTimer(void);

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif
