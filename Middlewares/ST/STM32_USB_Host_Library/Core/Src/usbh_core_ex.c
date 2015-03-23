#include "usbh_core_ex.h"


/*
 * This function setup communication pipes and kick start host gstate machine.
 */
static USBH_StatusTypeDef USBH_HandlePortUp(USBH_HandleTypeDef *phost)
{
  // ASSERT(phost->pState == PORT_UP);
  // ASSERT(phost->gState == HOST_IDLE);

  USBH_UsrLog("%s", __func__);

  phost->device.speed = USBH_LL_GetSpeed(phost);
  phost->gState = HOST_ENUMERATION;

  /* Debug output:
   * USBH_AllocPipe ep_addr 0000 pipe 0
   * USBH_AllocPipe ep_addr 0080 pipe 1
   */
  phost->Control.pipe_out = USBH_AllocPipe(phost, 0x00);
  phost->Control.pipe_in = USBH_AllocPipe(phost, 0x80);

  /* Open Control pipes */
  USBH_OpenPipe(phost, phost->Control.pipe_in, 0x80, phost->device.address,
      phost->device.speed,
      USBH_EP_CONTROL, phost->Control.pipe_size);

  /* Open Control pipes */
  USBH_OpenPipe(phost, phost->Control.pipe_out, 0x00, phost->device.address,
      phost->device.speed,
      USBH_EP_CONTROL, phost->Control.pipe_size);

  /** this field is only meaningful for upper layer apps **/
  phost->device.is_connected = 1;

  /* do this here, instead in connection handler **/
  if(phost->pUser != NULL)
  {
    phost->pUser(phost, HOST_USER_CONNECTION);
  }

#if (USBH_USE_OS == 1)
    osMessagePut ( phost->os_event, USBH_PORT_EVENT, 0);
#endif

  return USBH_OK;
}

/*
 *
 */
static USBH_StatusTypeDef  USBH_HandlePortDown(USBH_HandleTypeDef *phost) {

  // ASSERT(phost->pState == PORT_UP);
  // ASSERT(phost->gState != HOST_IDLE);

  USBH_LL_Stop(phost);

  /* Re-Initialize Host for new Enumeration */
  if (phost->pActiveClass != NULL)
  {
    phost->pActiveClass->DeInit(phost);
    phost->pActiveClass = NULL;
  }

  USBH_ClosePipe(phost, phost->Control.pipe_in);
  USBH_ClosePipe(phost, phost->Control.pipe_out);

  USBH_FreePipe(phost, phost->Control.pipe_in);
  USBH_FreePipe(phost, phost->Control.pipe_out);

  /** gstate -> HOST_IDLE **/
  DeInitGStateMachine(phost);

  phost->device.is_connected = 0;

  if(phost->pUser != NULL)
  {
    phost->pUser(phost, HOST_USER_DISCONNECTION);
  }

  USBH_Delay(100);

  USBH_LL_Start(phost);

  restore_debug_defaults();

  return USBH_OK;
}

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
  case PORT_IDLE:
    if (e.evt == USBH_EVT_NULL) {
    }
    else if (e.evt == USBH_EVT_CONNECT) {
      phost->device.is_connected = 1;
      USBH_Process(phost);
    }
    else
    {
      USBH_ERRORSTATE(phost, e);
    };
    break;

  case PORT_WAIT_PORT_UP:
    if (e.evt == USBH_EVT_NULL) {
      if (HAL_GetTick() - phost->pStateTimer > USBH_ATTACH_DELAY) {
        // TODO timeout processing
      }
    }
    else if (e.evt == USBH_EVT_PORTUP) {
      USBH_Delay(100);
      phost->gState = HOST_ENUMERATION;
      USBH_HandlePortUp(phost);
    }
    else if (e.evt == USBH_EVT_DISCONNECT) {
      phost->gState = HOST_IDLE;
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
      USBH_HandlePortDown(phost);
      phost->gState = HOST_DEV_DISCONNECTED;
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;

  case PORT_DOWN:
    if (e.evt == USBH_EVT_NULL) {
    }
    else if (e.evt == USBH_EVT_DISCONNECT) {
      phost->gState = HOST_IDLE;
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;
  }

  return USBH_OK;
}
