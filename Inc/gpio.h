/**
  ******************************************************************************
  * File Name          : gpio.h
  * Date               : 04/02/2015 16:50:31
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

 //NCP380LSN10AAT1G  Enable GPIO  PC2
 #define   VBUS_SWITCH_GPIO                 GPIO_PIN_2
 #define   VBUS_SWITCH_PORT   							 GPIOC
 #define   VBUS_SWITCH_GPIO_CLK_ENABLE()    __GPIOC_CLK_ENABLE()


 //TPS63010 Enable GPIO  PC1
 #define   BOOST5V_GPIO                 GPIO_PIN_1
 #define   BOOST5V_PORT   							 GPIOC
 #define   BOOST5V_GPIO_CLK_ENABLE()    __GPIOC_CLK_ENABLE()

 //TPS63010 Enable GPIO  PC1
 #define   VBUS_OVFLAG_GPIO                 GPIO_PIN_3
 #define   VBUS_OVFLAG_PORT   						 GPIOC
 #define   VBUS_OVFLAG__CLK_ENABLE()    	   __GPIOC_CLK_ENABLE()
 #define   VBUS_OVFLAG_IRQ                  EXTI3_IRQn
 #define   VBUS_OVFLAG_IRQHandler           EXTI3_IRQHandler
void MX_GPIO_Init(void);
void VBUS_Switch_Gpio_Init(void);
void Enable_VUSB_Switch(void);
void Boost5V_Gpio_Init(void);
void Enable_Boost5V(void);
void Disable_Boost5V(void);
void Vbus_OverflowFlag_Gpio_Init(void);
#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
