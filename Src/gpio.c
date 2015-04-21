/**
  ******************************************************************************
  * File Name          : gpio.c
  * Date               : 04/02/2015 16:50:30
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "stm32f4xx_it.h"
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOH_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();


  /*Configure GPIO pin : PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

/*
  VBUS_Switch_Gpio_Init();
  Boost5V_Gpio_Init();
*/
}

void VBUS_Switch_Gpio_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /*Configure GPIO pin : PC3 */
  VBUS_SWITCH_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = VBUS_SWITCH_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(VBUS_SWITCH_PORT, &GPIO_InitStruct);
}

void Enable_VUSB_Switch(void)
{
	HAL_GPIO_WritePin(VBUS_SWITCH_PORT, VBUS_SWITCH_GPIO, GPIO_PIN_SET);
}
void Disable_VBUS_Switch(void)
{
	HAL_GPIO_WritePin(VBUS_SWITCH_PORT, VBUS_SWITCH_GPIO, GPIO_PIN_RESET);
}



void Boost5V_Gpio_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /*Configure GPIO pin : PC3 */
  BOOST5V_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = BOOST5V_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BOOST5V_PORT, &GPIO_InitStruct);
}

void Enable_Boost5V(void)
{
	HAL_GPIO_WritePin(BOOST5V_PORT, BOOST5V_GPIO, GPIO_PIN_SET);
}
void Disable_Boost5V(void)
{
	HAL_GPIO_WritePin(BOOST5V_PORT, BOOST5V_GPIO, GPIO_PIN_RESET);
}

void Vbus_OverflowFlag_Gpio_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /*Configure GPIO pin : PC3 */
  BOOST5V_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = VBUS_OVFLAG_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_OVFLAG_PORT, &GPIO_InitStruct);

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(VBUS_OVFLAG_IRQ, 2, 0);
//  HAL_NVIC_EnableIRQ(VBUS_OVFLAG_IRQHandler);
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
