#ifndef __STM32F4XX_HAL_HCD_HELPER_H
#define __STM32F4XX_HAL_HCD_HELPER_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define USBH_CONNECT_DEBOUNCING_TICK          (3)

void hc_helper_prepare_reports(HCD_HandleTypeDef *hhcd, uint32_t interrupt);
void hc_helper_report_channel_in(uint32_t i);
void hc_helper_report_channel_out(uint32_t i);
void hc_helper_send_reports(void);
void hc_helper_sumbit_request(void);

//int is_debouncing(HCD_HandleTypeDef *hhcd);
//void start_debouncing(HCD_HandleTypeDef* hhcd);
//void hcd_debounce(HCD_HandleTypeDef *hhcd);



void HCD_DevState_Reset(HCD_HandleTypeDef *hhcd);
int HCD_DevState_IsConnected(HCD_HandleTypeDef *hhcd);
int HCD_DevState_IsAttached(HCD_HandleTypeDef *hhcd);
void HCD_DevState_Task(HCD_HandleTypeDef *hhcd);

#endif
