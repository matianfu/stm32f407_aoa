#include "stm32f4xx_hal.h"
//#include "time.h"


TIM_HandleTypeDef htim2;
static volatile uint32_t TimeCount = 0;

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM2_IRQn

void time2_init(void)
{

	  TIM_ClockConfigTypeDef sClockSourceConfig;
	  GPIO_InitTypeDef GPIO_InitStruct;
	
	
	  htim2.Instance = TIM2;
	  htim2.Init.Prescaler = 84 - 1;	  //1us
	  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim2.Init.Period = 1000-1;			  //10us
	  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim2.Init.RepetitionCounter = 0;
	  HAL_TIM_Base_Init(&htim2);
	
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);



  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM2)
  {
    /* Peripheral clock enable */
    __TIM2_CLK_ENABLE();
		  /*##-2- Configure the NVIC for TIMx ########################################*/
 	 /* Set Interrupt Group Priority */ 
  	HAL_NVIC_SetPriority(TIMx_IRQn, 4, 0);
  
 	 /* Enable the TIMx global Interrupt */
  	HAL_NVIC_EnableIRQ(TIMx_IRQn);
  }
}




#if 0
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {
    /* Peripheral clock disable */
    __TIM2_CLK_DISABLE();

  }
} 
#endif

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

#if 0
	static uint8_t flag = 0;

	if(htim == &htim2)
	{
		if(flag)
		{
			flag = 0;
			HAL_GPIO_WritePin( GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		}
		else
		{
			flag = 1 ;	
			HAL_GPIO_WritePin( GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		}

			
			 
	}
#endif
	TimeCount ++;
}


void StartTimeCount(void)
{
	TimeCount = 0;	
	HAL_TIM_Base_Start_IT(&htim2);
}
void StopTimeCount(void)
{
	TimeCount = 0;
	HAL_TIM_Base_Stop_IT(&htim2);		
}


uint32_t GetTimeCount(void)
{
	return TimeCount;
}





