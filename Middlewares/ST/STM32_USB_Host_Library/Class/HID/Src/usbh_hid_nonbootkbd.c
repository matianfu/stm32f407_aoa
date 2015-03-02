#include "usbh_def.h"
#include "usbh_hid.h"
#include "usbh_hid_nonbootkbd.h"

#define NB_KBD_DATA_SIZE                256

uint8_t nonboot_kbd_data[NB_KBD_DATA_SIZE];

USBH_StatusTypeDef USBH_HID_NonBootKbdInit(USBH_HandleTypeDef *phost) {

  HID_HandleTypeDef *HID_Handle =  phost->pActiveClass->pData;
  HID_Handle->pData = (uint8_t*)nonboot_kbd_data;
  return USBH_OK;
}


