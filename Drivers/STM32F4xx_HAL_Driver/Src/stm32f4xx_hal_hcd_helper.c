/*
 * This file is the helper module for stm32f3xx (usb) hcd hal debugging.
 */
#include <stdio.h>
#include "stm32f407xx.h"
#include "usbh_core.h"
#include "usbh_conf.h"
#include "debug.h"
#include "stm32f4xx_hal_hcd_helper.h"
#include "stm32f4xx_ll_usb.h"
#include "usbh_core_helper.h"

/*
 * Debug configuration
 */
#define DEBUG_HC_HAINT_ALL      ((uint32_t)0x0000FFFF)


// uint32_t debug_hc_haint_mask = 0;                       /** init as none enabled **/
uint32_t debug_hc_hcintx_mask[16] = {
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT,
    DEBUG_HC_HCINTX_MASK_DEFAULT};                      /** init as none enabled **/

static struct hcint_t hcint[16];
static struct hcint_t hcint_last = {0};

static uint32_t hc_report_channel[16];
static unsigned int debug_hc_uid = 0;

int debug_hal_hcd_hc_submitrequest_print = DEBUG_HAL_HCD_HC_SUBMITREQUEST_DEFAULT;
int debug_hal_hcd_hc_submitrequest_halt = 0;

static HCD_HandleTypeDef *hhcd;
static USB_OTG_GlobalTypeDef *USBx;
static uint32_t interrupt;

void hc_helper_prepare_reports(HCD_HandleTypeDef *_hhcd, uint32_t _interrupt)
{
  int i;
  hhcd = _hhcd;
  interrupt = _interrupt;
  USBx = hhcd->Instance;

  for (i = 0; i < hhcd->Init.Host_channels ; i++)
  {
    if (interrupt & (1 << i))
    {
      hc_report_channel[i] = (debug_hc_hcintx_mask[i] & (USBx_HC(i)->HCINT));
    }
    else {
      hc_report_channel[i] = 0;
    }
  }
}

void hc_helper_report_channel_in(uint32_t i)
{
  if (hc_report_channel[i])
  {
    memset(&hcint[i], 0, sizeof(hcint[i]));
    hcint[i].interrupt = interrupt;
    hcint[i].hcint_reg = (USBx_HC(i)->HCINT);
    hcint[i].in_state = hhcd->hc[i].state;
    hcint[i].in_urbstate = hhcd->hc[i].urb_state;
    hcint[i].in_err_count = hhcd->hc[i].ErrCnt;
    hcint[i].uid = debug_hc_uid++;

    if ((USBx_HC(i)->HCCHAR) &  USB_OTG_HCCHAR_EPDIR)
    {
      hcint[i].direction = 1; // IN
    }
    else {
      hcint[i].direction = 0; // OUT
    }
  }
}

void hc_helper_report_channel_out(uint32_t i)
{
  if (hc_report_channel[i]) {
    hcint[i].out_state = hhcd->hc[i].state;
    hcint[i].out_urbstate = hhcd->hc[i].urb_state;
    hcint[i].out_err_count = hhcd->hc[i].ErrCnt;
  }
}

USBH_StatusTypeDef usbh_ll_hcint(USBH_HandleTypeDef *phost, struct hcint_t * hcint) {

  USBH_EventTypeDef* e = USBH_AllocEvent();
  if (e) {
    e->evt = USBH_EVT_DISCONNECT;
    e->data.hcint = *hcint;
    USBH_SendEvent(e);
  }
  return USBH_OK;
}

void hc_helper_send_reports(void)
{
  uint32_t i;

  for (i = 0; i < hhcd->Init.Host_channels ; i++) {
    if (hc_report_channel[i]) {

      // skip duplicate hc report
      if (hcint[i].hcint_reg == hcint_last.hcint_reg &&
          hcint[i].in_state == hcint_last.in_state &&
          hcint[i].out_state == hcint_last.out_state &&
          hcint[i].in_urbstate == hcint_last.in_urbstate &&
          hcint[i].out_urbstate == hcint_last.out_urbstate &&
          hcint[i].in_err_count == hcint_last.in_err_count &&
          hcint[i].out_err_count == hcint_last.out_err_count) {
      }
      else {
        usbh_ll_hcint(hhcd->pData, &hcint[i]);
      }

      memcpy(&hcint_last, &hcint[i], sizeof(hcint_last));
    }
  }
}

void hc_helper_sumbit_request(void)
{
  if (debug_hal_hcd_hc_submitrequest_halt) {
    debug_hal_hcd_hc_submitrequest_halt = 0;
  }

  if (debug_hal_hcd_hc_submitrequest_print) {
    USBH_UsrLog("%s: ", __func__);
  }
}

void HCD_DevState_Reset(HCD_HandleTypeDef *hhcd)
{
  USBH_PutMessage("DevState: reset");
  hhcd->DevState.value = 0;
}

int HCD_DevState_IsConnected(HCD_HandleTypeDef *hhcd)
{
  return hhcd->DevState.state.connected ? 1 : 0;
}

int HCD_DevState_IsAttached(HCD_HandleTypeDef *hhcd)
{
  return hhcd->DevState.state.attached ? 1 : 0;
}

void HCD_DevState_Task(HCD_HandleTypeDef *hhcd)
{
  USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;

  hhcd->DevState.state.debounce = (hhcd->DevState.state.debounce << 1);

  if (USBx_HPRT0 & USB_OTG_HPRT_PCSTS)
  {
    hhcd->DevState.state.debounce |= 0x0001;
  }
  else
  {
    hhcd->DevState.state.debounce &= ~(0x0001);
  }

  if (hhcd->DevState.state.connected == 0)
  {
    if (hhcd->DevState.state.debounce == 0xFFFF)
    {
      USBH_PutMessage("DevState: connected.");
      hhcd->DevState.state.connected = 1;
    }
  }
  else
  {
    if (hhcd->DevState.state.debounce == 0x0000)
    {
      USBH_PutMessage("DevState: disconnected.");
      hhcd->DevState.state.connected = 0;
    }
  }
}

