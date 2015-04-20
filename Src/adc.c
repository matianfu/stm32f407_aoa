#include "adc.h"
#include "stdio.h"

ADC_HandleTypeDef AdcHandle;
BatteryTypeDef BatteryInfor;
void ADC_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;

	/*##-1- Configure the ADC peripheral #######################################*/
	AdcHandle.Instance = ADC1;

	AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	AdcHandle.Init.Resolution = ADC_RESOLUTION12b;
	AdcHandle.Init.ScanConvMode = DISABLE;
	AdcHandle.Init.ContinuousConvMode = DISABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion = 0;
	AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	AdcHandle.Init.NbrOfConversion = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection = DISABLE;

	HAL_ADC_Init(&AdcHandle);

	/*##-2- Configure ADC regular channel ######################################*/
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig.Offset = 0;

	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
}

/**
 * @brief ADC MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* ADC1 Periph clock enable */
	__ADC1_CLK_ENABLE();
	/* Enable GPIO clock ****************************************/
	__GPIOA_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* ADC1 Channel8 GPIO pin configuration */
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void Battery_Process(void)
{
	if (BatteryInfor.timeCount % 1000 == 0)
	{
		HAL_ADC_Start(&AdcHandle);
		HAL_ADC_PollForConversion(&AdcHandle, 10);
		if (HAL_ADC_GetState(&AdcHandle) == HAL_ADC_STATE_EOC_REG)
		{
			/*##-5- Get the converted value of regular channel #######################*/
			BatteryInfor.vol = 3300 * HAL_ADC_GetValue(&AdcHandle) / 4096 *2;
			BatteryInfor.pct = BatteryInfor.vol * 100 / 4200;

			printf("vol = %d, pct= %d\r\n", BatteryInfor.vol, BatteryInfor.pct);
		}
	}
}

