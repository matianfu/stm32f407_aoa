#ifndef __ADC_H_
#define __ADC_H_
#include "stm32f4xx_hal.h"

typedef struct
{
	uint16_t timeCount;
	uint16_t vol;
	uint16_t cap;
	uint16_t pct;
}BatteryTypeDef;

extern ADC_HandleTypeDef    AdcHandle;
extern BatteryTypeDef BatteryInfor;
void ADC_Init(void);
void Battery_Process(void);

#endif

