#include "usbh_core_ex.h"

/*
 * This function setup communication pipes and kick start host gstate machine.
 */
static USBH_StatusTypeDef USBH_HandlePortUp(USBH_HandleTypeDef *phost)
{

  ASSERT(phost->pState == PORT_UP);
  ASSERT(phost->gState == HOST_IDLE);

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

  ASSERT(phost->pState == PORT_UP);
  ASSERT(phost->gState != HOST_IDLE);

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

#if 1

  switch (phost->pState) {
  case PORT_IDLE:
    if (e.evt == USBH_EVT_NULL) {
    }
    else if (e.evt == USBH_EVT_CONNECT) {
      phost->pState = PORT_DEBOUNCE;
      phost->pStateTimer = HAL_GetTick();
      USBH_UsrLog("Debounce delay %dms before port reset", USBH_DEBOUNCE_DELAY);
    }
    else
    {
      USBH_ERRORSTATE(phost, e);
    };
    break;

  case PORT_DEBOUNCE:
    if (e.evt == USBH_EVT_NULL) {
      if (HAL_GetTick() - phost->pStateTimer > USBH_DEBOUNCE_DELAY) {
        phost->pState = PORT_RESET;

        /** huge bug !!! **/
        phost->pStateTimer = HAL_GetTick();
        USBH_LL_ResetAssert(phost);
        USBH_UsrLog("Assert USB port reset");
      }
    }
    else if (e.evt == USBH_EVT_DISCONNECT) {
      phost->pState = PORT_IDLE;
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;

  case PORT_RESET:
    if (e.evt == USBH_EVT_NULL) {
      // TODO ASSERT connected
      if (HAL_GetTick() - phost->pStateTimer > USBH_RESET_DURATION) {
        USBH_LL_ResetDeassert(phost);
        USBH_UsrLog("Deassert USB port reset");
        phost->pState = PORT_WAIT_ATTACHMENT;
        phost->pStateTimer = HAL_GetTick();
      }
    }
    else if (e.evt == USBH_EVT_DISCONNECT) {
      phost->pState = PORT_IDLE;
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;

  case PORT_WAIT_ATTACHMENT:

    if (e.evt == USBH_EVT_NULL) {
      if (HAL_GetTick() - phost->pStateTimer > USBH_ATTACH_DELAY) {
        // TODO timeout processing
      }
    }
    else if (e.evt == USBH_EVT_PORTUP) {
      phost->pState = PORT_UP_WAIT;
      phost->pStateTimer = HAL_GetTick();
    }
    else if (e.evt == USBH_EVT_DISCONNECT) {
      phost->pState = PORT_IDLE;
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;

  case PORT_UP_WAIT:
    if (e.evt == USBH_EVT_NULL) {
      if (HAL_GetTick() - phost->pStateTimer > 10) {
        phost->pState = PORT_UP;
        USBH_HandlePortUp(phost);
      }
    }
    else if (e.evt == USBH_EVT_PORTDOWN) {
      phost->pState = PORT_DOWN;
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
      phost->pState = PORT_DOWN;
    }
    else if (e.evt == USBH_EVT_CONNECT) {
      // no way to deal with this event, not maskable either
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;

  case PORT_DOWN:
    if (e.evt == USBH_EVT_NULL) {
      /*
       * a bug states is observed several times for unreliable plug.
       * with OTG_HS_HPRT as 0x00041801
       *
       * PCSTS = 1
       * PCDET = 0
       * PENA = 0
       * PENCHGN = 0
       *
       * the port is indeed disabled but physical connection is present.
       */
      // TODO clear this bug, maybe detect it and then re-enumeration
      if (USBH_LL_PortStale(phost)) {
        phost->pState = PORT_RESET;   // try to reset again
        USBH_LL_ResetAssert(phost);
        phost->pStateTimer = HAL_GetTick();
      }
    }
    else if (e.evt == USBH_EVT_DISCONNECT) {
      phost->pState = PORT_DISCONNECT_DELAY;
      phost->pStateTimer = HAL_GetTick();
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;

  case PORT_DISCONNECT_DELAY:
    if (e.evt == USBH_EVT_NULL) {
      if (HAL_GetTick() - phost->pStateTimer > 500) {
        // TODO disconnect stabilized
        phost->pState = PORT_IDLE;
      }
    }
    else if (e.evt == USBH_EVT_CONNECT) {
      phost->pState = PORT_DEBOUNCE;
    }
    else if (e.evt == USBH_EVT_DISCONNECT) {
      // Do nothing. It occurs occasionally.
    }
    else {
      USBH_ERRORSTATE(phost, e);
    }
    break;
  }

#else

  switch (e.evt)
  {
  case USBH_EVT_NULL:
    USBH_Process(phost);
    break;

  case USBH_EVT_CONNECT:

    if (phost->gState == HOST_IDLE)
    {
      phost->gState = HOST_DEV_WAIT_FOR_ATTACHMENT;

      /** debouncing **/
      phost->wait_for_attachment_substate = 0;
      phost->PollingTimer = HAL_GetTick();
      USBH_UsrLog("Debounce delay %dms before port reset", USBH_DEBOUNCE_DELAY);
    }
    else

      goto ILLEGAL_STATE;

    break;

  case USBH_EVT_DISCONNECT:

    /** the wild way **/
    // HAL_NVIC_SystemReset();

    switch (phost->gState)
    {
    case HOST_IDLE:
      USBH_ERRORSTATE(phost, e);
      break;

    case HOST_DEV_WAIT_FOR_ATTACHMENT:
      if (phost->wait_for_attachment_substate == 0)
      { /** debouncing **/

      }
      else if (phost->wait_for_attachment_substate == 1)
      { /** resetting **/
        USBH_LL_ResetDeassert(phost);
      }
      else
      {

      }

      phost->wait_for_attachment_substate = -1;
      phost->gState = HOST_IDLE;
      break;

    default:
      USBH_UsrLog("USB Device disconnected");

      USBH_LL_Stop(phost);

      /* Re-Initilaize Host for new Enumeration */
      if (phost->pActiveClass != NULL)
      {
        phost->pActiveClass->DeInit(phost);
        phost->pActiveClass = NULL;
      }

      USBH_ClosePipe(phost, phost->Control.pipe_in);
      USBH_ClosePipe(phost, phost->Control.pipe_out);

      USBH_FreePipe(phost, phost->Control.pipe_in);
      USBH_FreePipe(phost, phost->Control.pipe_out);

      USBH_Delay(100);
      DeInitGStateMachine(phost);

      USBH_LL_Start(phost);

      restore_debug_defaults();

      break;
    }
    break;

  case USBH_EVT_PORTUP:

    switch (phost->gState)
    {
    case HOST_IDLE:
      USBH_ERRORSTATE(phost, e);
      break;
    case HOST_DEV_WAIT_FOR_ATTACHMENT:
      phost->gState = HOST_DEV_ATTACHED;
      break;
    default:
      USBH_ERRORSTATE(phost, e);
      break;
    }
    break;

  case USBH_EVT_PORTDOWN:
    switch (phost->gState)
    {
    case HOST_DEV_WAIT_FOR_ATTACHMENT:
      if (phost->wait_for_attachment_substate == 2)
      {
        /** do nothing **/
      }

      break;
    default:
      break;
    }
    break;

  case USBH_EVT_OVERFLOW:
    break;

  default:
    // TODO illegal event
    break;
  }

  return USBH_OK;

  ILLEGAL_STATE:

  printf("ERROR STATE / EVENT combination !!!!!!!!!!!!");
  USBH_ERRORSTATE(phost, e);

#endif

  return USBH_OK;
}
