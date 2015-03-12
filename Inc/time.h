
#ifndef __TIME_H
#define __TIME_H


#include "stm32f4xx_hal.h"


//extern TIM_HandleTypeDef htim2;

void time_init(void);
void StartTimeCount(void);
void StopTimeCount(void);
uint32_t GetTimeCount(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


#endif
