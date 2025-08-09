#include <USER_Timer.h>
#include <USER_LEDs.h>
#include <USER_PROCESSING.h>
#include "tim.h"

volatile uint32_t lastCaptureTick = 0;
volatile uint32_t countGeiger = 0;
volatile uint32_t lastTickBPW34 = 0;
volatile uint32_t countBPW34 = 0;

volatile uint8_t periodStateFlag = 0;
volatile uint32_t periodCounter = 0;
volatile uint32_t transPeriod = 10;

void USER_TIMER_StartPWM(void) {
	
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); 
	
}

void USER_TIMER_StartUserTimer(void) {
	
	  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	  HAL_TIM_Base_Start_IT(&htim4);
	
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        uint32_t now = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        uint32_t delta = (now >= lastCaptureTick) ? (now - lastCaptureTick) : (0xFFFF - lastCaptureTick + now);

        if (delta >= TIME_GAP_MIN_US)
        {
            countGeiger++;  
            lastCaptureTick = now;
        }
    }
		if(htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        uint32_t now = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        uint32_t delta = (now >= lastTickBPW34) ? (now - lastTickBPW34) : (0xFFFF - lastTickBPW34 + now);

        if (delta >= TIME_GAP_MIN_US)
        {
            countBPW34++;
            lastTickBPW34 = now;
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)  
    {
			periodCounter++;
       
        if (periodCounter >= transPeriod)
        {   
					  periodStateFlag = 1;
            periodCounter = 0;
            USER_PROCESSING_ProcessAndSend();
        }
				if(doseRateOfGeiger >= USV_THRESHOLD || doseRateOfBPW34 >= USV_THRESHOLD)
        {
					  turn_off_led(LED_GPIO_PORT_B, LED_GREEN_PIN);
            toggle_led(LED_GPIO_PORT_B, LED_BLUE_PIN);
        }
        else
        {   
					  turn_off_led(LED_GPIO_PORT_B, LED_BLUE_PIN);
            toggle_led(LED_GPIO_PORT_B, LED_GREEN_PIN);
        }
    }
}
