#ifndef STM32F4XX_HAL_HCD_HELPER_H
#define STM32F4XX_HAL_HCD_HELPER_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "usbh_core_helper.h"

#define USBH_CONNECT_DEBOUNCING_TICK          (3)

void hc_helper_prepare_reports(HCD_HandleTypeDef *hhcd, uint32_t interrupt);
void hc_helper_report_channel_in(uint32_t i);
void hc_helper_report_channel_out(uint32_t i);
void hc_helper_send_reports(void);
void hc_helper_sumbit_request(void);

int is_debouncing(HCD_HandleTypeDef *hhcd);
void start_debouncing(HCD_HandleTypeDef* hhcd);
void hcd_debounce(HCD_HandleTypeDef *hhcd);

#endif
