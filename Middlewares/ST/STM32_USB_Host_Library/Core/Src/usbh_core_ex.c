#include "usbh_core_ex.h"

/**
 *  @brief  USBH_ProcessEvent
 *      Background process of the USB Core with asynchronous event processing.
 *      The original USBH_Process function should be considered as NULL_EVENT event handler.
 *  @param  phost: Host Handle
 *  @retval USBH Status
 */
USBH_StatusTypeDef USBH_ProcessEvent(USBH_HandleTypeDef * phost)
{
  static USBH_EventTypeDef e;

  e = USBH_GetFilteredEvent(phost);

  switch(mapped_port_state(phost)) {
  case PORT_IDLE: // GUARD: is_attached = 0;
    if (e.evt == USBH_EVT_NULL) {
    }
    else if (e.evt == USBH_EVT_CONNECT) {
      phost->device.is_attached = 0;
      phost->device.is_connected = 1;
      USBH_Process(phost);
    }
    else
    {
      USBH_ERRORSTATE(phost, e);
    };
    break;

  case PORT_WAIT_PORT_UP: // notice there are two sub-states
    if (e.evt == USBH_EVT_NULL) {
      if (phost->device.is_attached == 1) {
        USBH_Process(phost);
      }
      else if (HAL_GetTick() - phost->pStateTimer > USBH_ATTACH_DELAY) {
        // TODO timeout processing
      }
    }
    else if (e.evt == USBH_EVT_PORTUP) {
      phost->device.is_attached = 1;
      USBH_Process(phost);
    }
    else if (e.evt == USBH_EVT_DISCONNECT) {
      phost->device.is_connected = 0;
      USBH_Process(phost);
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;

  case PORT_UP:
    if (e.evt == USBH_EVT_NULL) {
      USBH_Process(phost);
    }
    else if (e.evt == USBH_EVT_PORTDOWN) {
      phost->device.is_attached = 0;
      USBH_Process(phost);
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;

  case PORT_DOWN:
    if (e.evt == USBH_EVT_NULL) {
    }
    else if (e.evt == USBH_EVT_DISCONNECT) {
      phost->device.is_connected = 0;
      USBH_Process(phost);
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;
  }

  return USBH_OK;
}
