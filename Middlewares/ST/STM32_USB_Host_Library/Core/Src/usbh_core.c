/**
  ******************************************************************************
  * @file    usbh_core.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    19-June-2014
  * @brief   This file implements the functions for the core state machine process
  *          the enumeration and the control transfer process
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/

#include "debug.h"
#include "usbh_conf.h"
#include "usbh_core.h"

/*
 * Macro definition
 */
#define USBH_ADDRESS_DEFAULT                    0
#define USBH_ADDRESS_ASSIGNED                   1
#define USBH_MPS_DEFAULT                        0x40

#define USBH_ERRORSTATE(s, e)                   printf(NEW_LINE "!!! ERROR STATE: %d" NEW_LINE, __LINE__);  \
                                                USBH_DebugOutput(s, e, 1);                                  \
                                                printf(NEW_LINE NEW_LINE);

#define USBH_DEBOUNCE_DELAY                     200
#define USBH_RESET_DURATION                     15
#define USBH_ATTACH_DELAY                       200

#define SIZE_OF_ARRAY(array)                    (sizeof(array) / sizeof(array[0]))

extern USBH_StatusTypeDef USBH_AOA_Handshake(USBH_HandleTypeDef * phost);

/*
 * local constants
 */
const static char* event_string[] =
{ "USBH_EVT_NULL", "USBH_EVT_CONNECT", "USBH_EVT_DISCONNECT",
    "USBH_EVT_PORTUP", "USBH_EVT_PORTDOWN", "USBH_EVT_OVERFLOW",
    "USBH_EVT_HCINT" };

const static char* pstate_string[] =
{ "PORT_IDLE", "PORT_DEBOUNCE", "PORT_RESET", "PORT_WAIT_ATTACHMENT",
    "PORT_UP_WAIT", "PORT_UP", "PORT_DOWN", "PORT_DISCONNECT_DELAY" };

const static char* gstate_string[] =
{ "HOST_IDLE", "HOST_DEV_WAIT_FOR_ATTACHMENT", "HOST_DEV_ATTACHED",
    "HOST_DEV_DISCONNECTED", "HOST_DETECT_DEVICE_SPEED", "HOST_ENUMERATION",
    "HOST_CLASS_REQUEST", "HOST_INPUT", "HOST_SET_CONFIGURATION",
    "HOST_CHECK_CLASS", "HOST_HAND_SHAKE", "HOST_CLASS", "HOST_SUSPENDED",
    "HOST_ABORT_STATE" };

const static char* enum_state_string[] =
{ "ENUM_IDLE", "ENUM_GET_FULL_DEV_DESC", "ENUM_SET_ADDR", "ENUM_GET_CFG_DESC",
    "ENUM_GET_FULL_CFG_DESC", "ENUM_GET_MFC_STRING_DESC",
    "ENUM_GET_PRODUCT_STRING_DESC", "ENUM_GET_SERIALNUM_STRING_DESC" };

const static char* request_state_string[] =
{ "CMD_IDLE", "CMD_SEND", "CMD_WAIT" };


const static char* control_state_string[] =
{ "CTRL_IDLE", "CTRL_SETUP", "CTRL_SETUP_WAIT", "CTRL_DATA_IN",
    "CTRL_DATA_IN_WAIT", "CTRL_DATA_OUT", "CTRL_DATA_OUT_WAIT",
    "CTRL_STATUS_IN", "CTRL_STATUS_IN_WAIT", "CTRL_STATUS_OUT",
    "CTRL_STATUS_OUT_WAIT", "CTRL_ERROR", "CTRL_STALLED", "CTRL_COMPLETE" };
/**
  * @brief  URB States definition
  */
const static char * urb_state_string[] =
{ "URB_IDLE", // = 0,
    "URB_DONE", "URB_NOTREADY", "URB_NYET", "URB_ERROR", "URB_STALL" };

/**
  * @brief  Host channel States  definition
  */
const static char * channel_state_string[] =
{ "HC_IDLE", "HC_XFRC", "HC_HALTED", "HC_NAK", "HC_NYET", "HC_STALL",
    "HC_XACTERR", "HC_BBLERR", "HC_DATATGLERR" };

/*
 * local functions
 */

/*
 * @param force, forcefully print, otherwise successive same event print only once
 */
static void USBH_DebugOutput(USBH_HandleTypeDef* phost, USBH_EventTypeDef event, int force)
{
  static PORT_StateTypeDef ps = -1;
  static HOST_StateTypeDef gs = -1;
  static ENUM_StateTypeDef es = -1;
  static CMD_StateTypeDef rs = -1;
  static CTRL_StateTypeDef cs = -1;
  static USBH_EventTypeDef e = {-1, 0};
  static char time_strbuf[16];

  if (!force) {
    /** print only once for successive state/event **/
    if ((phost->pState == ps) &&
        (phost->gState == gs) &&
        (phost->EnumState == es) &&
        (phost->RequestState == rs) &&
        (phost->Control.state == cs) &&
        (e.evt == event.evt))
    {
      return;
    }
  }

  ps = phost->pState;
  gs = phost->gState;
  es = phost->EnumState;
  rs = phost->RequestState;
  cs = phost->Control.state;
  e.evt = event.evt;
  e.timestamp = event.timestamp;

  if (ps < SIZE_OF_ARRAY(pstate_string) &&
      gs < SIZE_OF_ARRAY(gstate_string) &&
      es < SIZE_OF_ARRAY(enum_state_string) &&
      rs < SIZE_OF_ARRAY(request_state_string) &&
      cs < SIZE_OF_ARRAY(control_state_string) &&
      e.evt < SIZE_OF_ARRAY(event_string))
  {
    if (e.evt == USBH_EVT_NULL) {
      time_strbuf[0] = '\0';
    }
    else {
      snprintf(time_strbuf, 16, "@ %08u", (unsigned int)e.timestamp);
    }

    USBH_UsrLog("- %s, %s, %s, %s, %s, %s %s",
        pstate_string[ps],
        gstate_string[gs],
        enum_state_string[es],
        request_state_string[rs],
        control_state_string[cs],
        event_string[e.evt],
        time_strbuf);
  }
  else
  {
    USBH_UsrLog("!!!! Illegal USBH States, s %d, es %d, rs %d, cs %d, e %d @ %010u",
        gs, es, rs, cs, e.evt, (unsigned int )e.timestamp);
  }
}

static void USBH_Print_DeviceDescriptor(USBH_HandleTypeDef *phost)
{
	int i;

	USBH_DevDescTypeDef* devdesc = &phost->device.DevDesc;
	USBH_CfgDescTypeDef* cfgdesc = &phost->device.CfgDesc;
	USBH_InterfaceDescTypeDef* itfdesc;
	int bNumInterfaces =  cfgdesc->bNumInterfaces;

	USBH_UsrLog("Device Descriptor:");
	USBH_UsrLog("  bLength:            %d", 	devdesc->bLength);
	USBH_UsrLog("  bDescriptionType:   %d", 	devdesc->bDescriptorType);
	USBH_UsrLog("  bcdUSB:             0x%04x", devdesc->bcdUSB);
	USBH_UsrLog("  bDeviceClass:       %d", 	devdesc->bDeviceClass);
	USBH_UsrLog("  bDeviceSubClass:    %d", 	devdesc->bDeviceSubClass);
	USBH_UsrLog("  bDeviceProtocol:    %d", 	devdesc->bDeviceProtocol);
	USBH_UsrLog("  bMaxPacketSize:     %d", 	devdesc->bMaxPacketSize);
	USBH_UsrLog("  idVendor:           0x%04x", devdesc->idVendor);
	USBH_UsrLog("  idProduct:          0x%04x", devdesc->idProduct);
	USBH_UsrLog("  bcdDevice:          0x%04x", devdesc->bcdDevice);
	USBH_UsrLog("  iManufacturer:      %d", 	devdesc->iManufacturer);
	USBH_UsrLog("  iProduct:           %d", 	devdesc->iProduct);
	USBH_UsrLog("  iSerial:            %d", 	devdesc->iSerialNumber);
	USBH_UsrLog("  bNumConfigurations: %d", 	devdesc->bNumConfigurations);

	USBH_UsrLog("  Configuration Descriptor:");
	USBH_UsrLog("    bLength:          %d",		cfgdesc->bLength);
	USBH_UsrLog("    bDescriptorType:  %d",     cfgdesc->bDescriptorType);
	USBH_UsrLog("    wTotalLength:     %d",	    cfgdesc->wTotalLength);
	USBH_UsrLog("    bNumInterfaces:   %d",     cfgdesc->bNumInterfaces);
	USBH_UsrLog("    bConfiguration:   %d",		cfgdesc->bConfigurationValue);
	USBH_UsrLog("    iConfiguration:   %d",     cfgdesc->iConfiguration);
	USBH_UsrLog("    bmAttributes:     0x%04x", cfgdesc->bmAttributes);
	USBH_UsrLog("    bMaxPower:        %d",     cfgdesc->bMaxPower);

for (i = 0; i < bNumInterfaces; i++) {
	itfdesc = &cfgdesc->Itf_Desc[i];

	USBH_UsrLog("    Interface Descriptor:");
	USBH_UsrLog("      bLength:                %d",		itfdesc->bLength);
	USBH_UsrLog("      bDescriptionType:       %d",		itfdesc->bDescriptorType);
	USBH_UsrLog("      bInterfaceNumber:       %d",		itfdesc->bInterfaceNumber);
	USBH_UsrLog("      bAlternateSetting:      %d", 	itfdesc->bAlternateSetting);
	USBH_UsrLog("      bNumEndpoints:          %d", 	itfdesc->bNumEndpoints);
	USBH_UsrLog("      bInterfaceClass:        %d", 	itfdesc->bInterfaceClass);
	USBH_UsrLog("      bInterfaceSubClass:     %d", 	itfdesc->bInterfaceSubClass);
	USBH_UsrLog("      bInterfaceProtocol:     %d", 	itfdesc->bInterfaceProtocol);
	USBH_UsrLog("      iInterface:             %d", 	itfdesc->iInterface);
}


}

void hcint2string(char* buf, uint32_t hcint)
{
  int i ;

  static const char* name[] =
  { "XFRC", "CHH", "AHBERR", "STALL", "NAK", "ACK", "NYET", "TXERR", "BBERR",
      "FRMOR", "DTERR" };

  for (i = 0; i < 11; i++) {

    if (hcint & (1 << i)) {
      strcpy(buf, name[i]);
      buf += strlen(name[i]);
      *buf = ' ';
      buf ++;
    }
  }

  /* remove last space */
  if (strlen(buf) > 0) {
    buf--;
    *buf = 0;
  }
}

/*
 * a ring buffer for asynchronous USBH event
 *
 * TODO these data should be changed to struct, since each USB (HS, FS) need an instance
 */
#define USBH_EVENT_RING_SIZE				(64)
static USBH_EventTypeDef USBH_Events[USBH_EVENT_RING_SIZE] = { {0, 0, {0}} };
static int get_event_index = 0;
static int put_event_index = 0;

static inline int next_event_index(int i) {

	if (i == USBH_EVENT_RING_SIZE - 1)
		return 0;

	return ++i;
}

static USBH_EventTypeDef USBH_GetEvent(void) {

  USBH_EventTypeDef e;

	if (get_event_index == put_event_index) {
		e.evt = USBH_EVT_NULL;
		e.timestamp = 0;
		return e;
	}

	e = USBH_Events[get_event_index];
	USBH_Events[get_event_index].evt = 0;
	USBH_Events[get_event_index].timestamp = 0;

	get_event_index = next_event_index(get_event_index);

	return e;
}

static void USBH_PutEvent(USBH_EventTypeDef e) {

	int i;
	if (next_event_index(put_event_index) == get_event_index) {

		for (i = 0; i < USBH_EVENT_RING_SIZE; i++)
			USBH_Events[i].evt = USBH_EVT_OVERFLOW;

		return;
	}

	USBH_Events[put_event_index] = e;
	put_event_index = next_event_index(put_event_index);
}

/** @defgroup USBH_CORE_Private_Functions
  * @{
  */ 
static USBH_StatusTypeDef  USBH_HandlePortUp(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef  USBH_HandlePortDown(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef  USBH_HandleEnum    (USBH_HandleTypeDef *phost);
static void                USBH_HandleSof     (USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef  DeInitGStateMachine(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef  DeInitPStateMachine(USBH_HandleTypeDef *phost);

#if (USBH_USE_OS == 1)  
static void USBH_Process_OS(void const * argument);
#endif

/**
  * @brief  HCD_Init 
  *         Initialize the HOST Core.
  * @param  phost: Host Handle
  * @param  pUsrFunc: User Callback
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_Init(USBH_HandleTypeDef *phost, void (*pUsrFunc)(USBH_HandleTypeDef *phost, uint8_t ), uint8_t id)
{
  /* Check whether the USB Host handle is valid */
  if(phost == NULL)
  {
    USBH_ErrLog("Invalid Host handle");
    return USBH_FAIL; 
  }
  
  /* Set DRiver ID */
  phost->id = id;
  
  /* Unlink class*/
  phost->pActiveClass = NULL;
  phost->ClassNumber = 0;
  
  /* Restore default states and prepare EP0 */ 
  DeInitPStateMachine(phost);
  
  /* Assign User process */
  if(pUsrFunc != NULL)
  {
    phost->pUser = pUsrFunc;
  }
  
#if (USBH_USE_OS == 1) 
  
  /* Create USB Host Queue */
  osMessageQDef(USBH_Queue, 10, uint16_t);
  phost->os_event = osMessageCreate (osMessageQ(USBH_Queue), NULL); 
  
  /*Create USB Host Task */
#if defined (USBH_PROCESS_STACK_SIZE)
  osThreadDef(USBH_Thread, USBH_Process_OS, USBH_PROCESS_PRIO, 0, USBH_PROCESS_STACK_SIZE);
#else
  osThreadDef(USBH_Thread, USBH_Process_OS, USBH_PROCESS_PRIO, 0, 8 * configMINIMAL_STACK_SIZE);
#endif  
  phost->thread = osThreadCreate (osThread(USBH_Thread), phost);
#endif  
  
  /* Initialize low level driver */
  USBH_LL_Init(phost);
  return USBH_OK;
}

/**
  * @brief  HCD_Init 
  *         De-Initialize the Host portion of the driver.
  * @param  phost: Host Handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_DeInit(USBH_HandleTypeDef *phost)
{
  DeInitPStateMachine(phost);
  
  if(phost->pData != NULL)
  {
    phost->pActiveClass->pData = NULL;
    USBH_LL_Stop(phost);
  }

  return USBH_OK;
}

/**
  * @brief  DeInitStateMachine 
  *         De-Initialize the Host state machine.
  * @param  phost: Host Handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef  DeInitGStateMachine(USBH_HandleTypeDef *phost)
{
  uint32_t i = 0;

  /* Clear Pipes flags*/
  for ( ; i < USBH_MAX_PIPES_NBR; i++)
  {
    phost->Pipes[i] = 0;
  }
  
  for(i = 0; i< USBH_MAX_DATA_BUFFER; i++)
  {
    phost->device.Data[i] = 0;
  }
  
  phost->gState = HOST_IDLE;
  phost->EnumState = ENUM_IDLE;
  phost->RequestState = CMD_SEND;
#ifdef DBGLOG_USBH_REQSTATE
    USBH_UsrLog("ReqState = CMD_SEND");
#endif
  phost->Timer = 0;  
  
  phost->Control.state = CTRL_SETUP;
  phost->Control.pipe_size = USBH_MPS_DEFAULT;  
  phost->Control.errorcount = 0;
  
  phost->device.address = USBH_ADDRESS_DEFAULT;
  phost->device.speed   = USBH_SPEED_FULL;
  
  return USBH_OK;
}

/*
 * This function also reset port state
 */
static USBH_StatusTypeDef  DeInitPStateMachine(USBH_HandleTypeDef *phost)
{
  DeInitGStateMachine(phost);
  phost->pState = PORT_IDLE;

  return USBH_OK;
}

/**
  * @brief  USBH_RegisterClass 
  *         Link class driver to Host Core.
  * @param  phost : Host Handle
  * @param  pclass: Class handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_RegisterClass(USBH_HandleTypeDef *phost, USBH_ClassTypeDef *pclass)
{
  USBH_StatusTypeDef   status = USBH_OK;
  
  if(pclass != 0)
  {
    if(phost->ClassNumber < USBH_MAX_NUM_SUPPORTED_CLASS)
    {
      /* link the class tgo the USB Host handle */
      phost->pClass[phost->ClassNumber++] = pclass;
      status = USBH_OK;
    }
    else
    {
      USBH_ErrLog("Max Class Number reached");
      status = USBH_FAIL; 
    }
  }
  else
  {
    USBH_ErrLog("Invalid Class handle");
    status = USBH_FAIL; 
  }
  
  return status;
}

/**
  * @brief  USBH_SelectInterface 
  *         Select current interface.
  * @param  phost: Host Handle
  * @param  interface: Interface number
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_SelectInterface(USBH_HandleTypeDef *phost, uint8_t interface)
{
  USBH_StatusTypeDef   status = USBH_OK;
  
  if(interface < phost->device.CfgDesc.bNumInterfaces)
  {
    phost->device.current_interface = interface;
    USBH_UsrLog ("Switching to Interface (#%d)", interface);
    USBH_UsrLog ("Class    : %xh", phost->device.CfgDesc.Itf_Desc[interface].bInterfaceClass );
    USBH_UsrLog ("SubClass : %xh", phost->device.CfgDesc.Itf_Desc[interface].bInterfaceSubClass );
    USBH_UsrLog ("Protocol : %xh", phost->device.CfgDesc.Itf_Desc[interface].bInterfaceProtocol );                 
  }
  else
  {
    USBH_ErrLog ("Cannot Select This Interface.");
    status = USBH_FAIL; 
  }
  return status;  
}

/**
  * @brief  USBH_GetActiveClass 
  *         Return Device Class.
  * @param  phost: Host Handle
  * @param  interface: Interface index
  * @retval Class Code
  */
uint8_t USBH_GetActiveClass(USBH_HandleTypeDef *phost)
{
   return (phost->device.CfgDesc.Itf_Desc[0].bInterfaceClass);            
}
/**
  * @brief  USBH_FindInterface 
  *         Find the interface index for a specific class.
  * @param  phost: Host Handle
  * @param  Class: Class code
  * @param  SubClass: SubClass code
  * @param  Protocol: Protocol code
  * @retval interface index in the configuration structure
  * @note : (1)interface index 0xFF means interface index not found
  */
uint8_t  USBH_FindInterface(USBH_HandleTypeDef *phost, uint8_t Class, uint8_t SubClass, uint8_t Protocol)
{
  USBH_InterfaceDescTypeDef    *pif ;
  USBH_CfgDescTypeDef          *pcfg ;
  int8_t                        if_ix = 0;
  
  pif = (USBH_InterfaceDescTypeDef *)0;
  pcfg = &phost->device.CfgDesc;  
  
  while (if_ix < USBH_MAX_NUM_INTERFACES)
  {
    pif = &pcfg->Itf_Desc[if_ix];
    if(((pif->bInterfaceClass == Class) || (Class == 0xFF))&&
       ((pif->bInterfaceSubClass == SubClass) || (SubClass == 0xFF))&&
         ((pif->bInterfaceProtocol == Protocol) || (Protocol == 0xFF)))
    {
      return  if_ix;
    }
    if_ix++;
  }
  return 0xFF;
}

/**
  * @brief  USBH_FindInterfaceIndex 
  *         Find the interface index for a specific class interface and alternate setting number.
  * @param  phost: Host Handle
  * @param  interface_number: interface number
  * @param  alt_settings    : alaternate setting number
  * @retval interface index in the configuration structure
  * @note : (1)interface index 0xFF means interface index not found
  */
uint8_t  USBH_FindInterfaceIndex(USBH_HandleTypeDef *phost, uint8_t interface_number, uint8_t alt_settings)
{
  USBH_InterfaceDescTypeDef    *pif ;
  USBH_CfgDescTypeDef          *pcfg ;
  int8_t                        if_ix = 0;
  
  pif = (USBH_InterfaceDescTypeDef *)0;
  pcfg = &phost->device.CfgDesc;  
  
  while (if_ix < USBH_MAX_NUM_INTERFACES)
  {
    pif = &pcfg->Itf_Desc[if_ix];
    if((pif->bInterfaceNumber == interface_number) && (pif->bAlternateSetting == alt_settings))
    {
      return  if_ix;
    }
    if_ix++;
  }
  return 0xFF;
}

/**
  * @brief  USBH_Start 
  *         Start the USB Host Core.
  * @param  phost: Host Handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_Start  (USBH_HandleTypeDef *phost)
{
  /* Start the low level driver  */
  USBH_LL_Start(phost);
  
  /* Activate VBUS on the port */ 
  USBH_LL_DriverVBUS (phost, TRUE);
  
  return USBH_OK;  
}

/**
  * @brief  USBH_Stop 
  *         Stop the USB Host Core.
  * @param  phost: Host Handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_Stop   (USBH_HandleTypeDef *phost)
{
  /* Stop and cleanup the low level driver  */
  USBH_LL_Stop(phost);  
  
  /* DeActivate VBUS on the port */ 
  USBH_LL_DriverVBUS (phost, FALSE);
  
  /* FRee Control Pipes */
//  USBH_FreePipe  (phost, phost->Control.pipe_in);
//  USBH_FreePipe  (phost, phost->Control.pipe_out);
  
  return USBH_OK;  
}

/**
  * @brief  HCD_ReEnumerate 
  *         Perform a new Enumeration phase.
  * @param  phost: Host Handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_ReEnumerate   (USBH_HandleTypeDef *phost)
{
  /*Stop Host */ 
  USBH_Stop(phost);

  /*Device has disconnected, so wait for 200 ms */  
  USBH_Delay(200);
  
  /* Set State machines in default state */
  DeInitGStateMachine(phost);
   
  /* Start again the host */
  USBH_Start(phost);
      
#if (USBH_USE_OS == 1)
      osMessagePut ( phost->os_event, USBH_PORT_EVENT, 0);
#endif  
  return USBH_OK;  
}





/**
 * 	@brief	USBH_ProcessEvent
 * 			Background process of the USB Core with asynchronous event processing.
 * 			The original USBH_Process function should be considered as NULL_EVENT event handler.
 * 	@param	phost: Host Handle
 * 	@retval USBH Status
 */
USBH_StatusTypeDef USBH_ProcessEvent(USBH_HandleTypeDef * phost)
{
  static USBH_EventTypeDef e;

  static char buf[128];

pop:
  e = USBH_GetEvent();

  if (e.evt == USBH_EVT_HCINT) {

    memset(buf, 0, 128);

    hcint2string(buf, e.data.hcint.hcint_reg);

    printf(" :: HCINT %08x, %02x, %04x, %s, %s, %s %s, %s %s, %d %d, - %08u" NEW_LINE,
        e.data.hcint.uid,
        (unsigned int)e.data.hcint.interrupt,
        (unsigned int)e.data.hcint.hcint_reg,
        buf,
        ((e.data.hcint.direction == 0) ? "OUT" : "IN"),
        channel_state_string[e.data.hcint.in_state],
        channel_state_string[e.data.hcint.out_state],
        urb_state_string[e.data.hcint.in_urbstate],
        urb_state_string[e.data.hcint.out_urbstate],
        e.data.hcint.in_err_count,
        e.data.hcint.out_err_count,
        (unsigned int)e.timestamp);

    goto pop;
  }

  USBH_DebugOutput(phost, e, 0);

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

/**
  * @brief  USBH_Process 
  *         Background process of the USB Core.
  * @param  phost: Host Handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_Process(USBH_HandleTypeDef *phost)
{
  __IO USBH_StatusTypeDef status = USBH_FAIL;
  uint8_t idx = 0, j;

  switch (phost->gState)
  {
  case HOST_IDLE:
    break;

//  case HOST_DEV_WAIT_FOR_ATTACHMENT:
//
//    if (phost->wait_for_attachment_substate == 0)
//    { /** Debouncing **/
//
//      if (HAL_GetTick() - phost->PollingTimer > USBH_DEBOUNCE_DELAY)
//      {
//        phost->wait_for_attachment_substate = 1; /** switching substate **/
//        phost->PollingTimer = HAL_GetTick();
//        USBH_LL_ResetAssert(phost);
//        USBH_UsrLog("Assert USB port reset");
//      }
//    }
//    else if (phost->wait_for_attachment_substate == 1) /** resetting **/
//    {
//      if (HAL_GetTick() - phost->PollingTimer > 12)
//      { /** 10 - 20ms conforming to standard **/
//        USBH_LL_ResetDeassert(phost);
//        USBH_UsrLog("Deassert USB port reset");
//        phost->wait_for_attachment_substate = 2;
//      }
//    }
//    else
//    {
//      // TODO waiting for port up event, need timeout mechanism
//    }
//    break;

//  case HOST_DEV_ATTACHED:
//
//    USBH_UsrLog("USB Device Attached")
//    ;
//
//    /* Wait for some time after Reset */
//    USBH_Delay(USBH_ATTACH_DELAY);
//
//    phost->device.speed = USBH_LL_GetSpeed(phost);
//
//    phost->gState = HOST_ENUMERATION;
//
//    /*
//     * USBH_AllocPipe ep_addr 0000 pipe 0
//     * USBH_AllocPipe ep_addr 0080 pipe 1
//     */
//    phost->Control.pipe_out = USBH_AllocPipe(phost, 0x00);
//    phost->Control.pipe_in = USBH_AllocPipe(phost, 0x80);
//
//    /* Open Control pipes */
//    USBH_OpenPipe(phost, phost->Control.pipe_in, 0x80, phost->device.address,
//        phost->device.speed,
//        USBH_EP_CONTROL, phost->Control.pipe_size);
//
//    /* Open Control pipes */
//    USBH_OpenPipe(phost, phost->Control.pipe_out, 0x00, phost->device.address,
//        phost->device.speed,
//        USBH_EP_CONTROL, phost->Control.pipe_size);
//
//#if (USBH_USE_OS == 1)
//    osMessagePut ( phost->os_event, USBH_PORT_EVENT, 0);
//#endif
//
//    break;

  case HOST_ENUMERATION:
    /* Check for enumeration status */
    if (USBH_OK == (status = USBH_HandleEnum(phost)))
    {
      /* The function shall return USBH_OK when full enumeration is complete */
      USBH_UsrLog("Enumeration done.");

      USBH_Print_DeviceDescriptor(phost);

      phost->device.current_interface = 0;
      if (phost->device.DevDesc.bNumConfigurations == 1)
      {
        USBH_UsrLog("This device has only 1 configuration.");
        phost->gState = HOST_SET_CONFIGURATION;

      }
      else
      {
        phost->gState = HOST_INPUT;
      }

    }

    break;

  case HOST_INPUT:
  {
    /* user callback for end of device basic enumeration */
    if (phost->pUser != NULL)
    {
      phost->pUser(phost, HOST_USER_SELECT_CONFIGURATION);
      phost->gState = HOST_SET_CONFIGURATION;

#if (USBH_USE_OS == 1)
      osMessagePut ( phost->os_event, USBH_STATE_CHANGED_EVENT, 0);
#endif         
    }
  }
    break;

  case HOST_SET_CONFIGURATION:
    /* set configuration */
    if (USBH_SetCfg(phost, phost->device.CfgDesc.bConfigurationValue) == USBH_OK)
    {
      USBH_UsrLog("Default configuration set.");

      if (phost->ClassNumber == 0)
      {

        USBH_UsrLog("No Class has been registered.");
        phost->gState = HOST_ABORT_STATE;
      }
      else
      {

        USBH_UsrLog("Checking class.")
        phost->gState = HOST_CHECK_CLASS;
        phost->pActiveClass = NULL;
      }
    }
    break;

  case HOST_CHECK_CLASS:

    // for (idx = 0; idx < USBH_MAX_NUM_SUPPORTED_CLASS ; idx ++)
    for (idx = 0; idx < phost->ClassNumber; idx++)
    {
      // search all interfaces, not only interface 0
      for (j = 0; j < phost->device.CfgDesc.bNumInterfaces; j++)
      {

        if (phost->pClass[idx]->ClassCode
            == phost->device.CfgDesc.Itf_Desc[j].bInterfaceClass)
        {
          phost->pActiveClass = phost->pClass[idx];
          USBH_UsrLog(
              "Registered %s class with code 0x%02x matches interface #%d",
              phost->pClass[idx]->Name, phost->pClass[idx]->ClassCode, j);
          break;
        }
      }

      if (phost->pActiveClass != NULL)
        break;
    }

    if (phost->pActiveClass != NULL)
    {
      if (phost->pActiveClass->Init(phost) == USBH_OK)
      {
        phost->gState = HOST_CLASS_REQUEST;
        USBH_UsrLog("%s class started.", phost->pActiveClass->Name);

        /* Inform user that a class has been activated */
        phost->pUser(phost, HOST_USER_CLASS_SELECTED);
      }
      else
      {
        phost->gState = HOST_HAND_SHAKE;
        USBH_UsrLog("Device not supporting %s class, try handshake.",
            phost->pActiveClass->Name);
      }
    }
    else
    {
      /** switch to abort state **/
      // phost->gState  = HOST_ABORT_STATE;
      // USBH_UsrLog ("No registered class for this device.");
      phost->gState = HOST_HAND_SHAKE;
      USBH_UsrLog("No registered class for this device, try handshake.");
    }

#if (USBH_USE_OS == 1)
    osMessagePut ( phost->os_event, USBH_STATE_CHANGED_EVENT, 0);
#endif 
    break;

  case HOST_HAND_SHAKE:

    status = USBH_AOA_Handshake(phost);

    if (status == USBH_FAIL || status == USBH_NOT_SUPPORTED)
    {
      phost->gState = HOST_ABORT_STATE;
      USBH_UsrLog("Handshake fail, abort.");
    }

    break;

  case HOST_CLASS_REQUEST:

    /* process class standard control requests state machine */
    if (phost->pActiveClass != NULL)
    {
      /* as long as pActiveClass is set, this state is  */
      status = phost->pActiveClass->Requests(phost);

      if (status == USBH_OK)
      {
        phost->gState = HOST_CLASS;
      }
      else if (status == USBH_FAIL || status == USBH_NOT_SUPPORTED)
      {

        phost->gState = HOST_ABORT_STATE;
        USBH_ErrLog("Class Request fail.")
      }
    }
    else
    {
      phost->gState = HOST_ABORT_STATE;
      USBH_ErrLog("Invalid Class Driver.");

#if (USBH_USE_OS == 1)
      osMessagePut ( phost->os_event, USBH_STATE_CHANGED_EVENT, 0);
#endif       
    }

    break;
  case HOST_CLASS:
    /* process class state machine */
    if (phost->pActiveClass != NULL)
    {
      phost->pActiveClass->BgndProcess(phost);
    }
    break;

//  case HOST_DEV_DISCONNECTED:
//
//    DeInitGStateMachine(phost);
//
//    /* Re-Initilaize Host for new Enumeration */
//    if (phost->pActiveClass != NULL)
//    {
//      phost->pActiveClass->DeInit(phost);
//      phost->pActiveClass = NULL;
//    }
//    break;
  case HOST_ABORT_STATE:
    break;

  default:
    USBH_UsrLog("Unhandled host state: %s", gstate_string[phost->gState]);
    break;
  }

  return USBH_OK;
}


/**
  * @brief  USBH_HandleEnum 
  *         This function includes the complete enumeration process
  * @param  phost: Host Handle
  * @retval USBH_Status
  */
static USBH_StatusTypeDef USBH_HandleEnum (USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef Status = USBH_BUSY;  
  
  switch (phost->EnumState)
  {
  case ENUM_IDLE:
    /* Get Device Desc for only 1st 8 bytes : To get EP0 MaxPacketSize */
    if ( USBH_Get_DevDesc(phost, 8) == USBH_OK)
    {
      phost->Control.pipe_size = phost->device.DevDesc.bMaxPacketSize;

      phost->EnumState = ENUM_GET_FULL_DEV_DESC;
      
      /* modify control channels configuration for MaxPacket size */
      USBH_OpenPipe(phost, phost->Control.pipe_in, 0x80, phost->device.address,
          phost->device.speed,
          USBH_EP_CONTROL, phost->Control.pipe_size);

      /* Open Control pipes */
      USBH_OpenPipe(phost, phost->Control.pipe_out, 0x00, phost->device.address,
          phost->device.speed,
          USBH_EP_CONTROL, phost->Control.pipe_size);
      
    }
    break;
    
  case ENUM_GET_FULL_DEV_DESC:  
    /* Get FULL Device Desc  */
    if ( USBH_Get_DevDesc(phost, USB_DEVICE_DESC_SIZE)== USBH_OK)
    {
      USBH_UsrLog("PID: %xh", phost->device.DevDesc.idProduct );  
      USBH_UsrLog("VID: %xh", phost->device.DevDesc.idVendor );  
      
      phost->EnumState = ENUM_SET_ADDR;
       
    }
    break;
   
  case ENUM_SET_ADDR: 
    /* set address */
    if ( USBH_SetAddress(phost, USBH_DEVICE_ADDRESS) == USBH_OK)
    {
      USBH_Delay(2);
      phost->device.address = USBH_DEVICE_ADDRESS;
      
      /* user callback for device address assigned */
      USBH_UsrLog("Address (#%d) assigned.", phost->device.address);
      phost->EnumState = ENUM_GET_CFG_DESC;
      
      /* modify control channels to update device address */
      USBH_OpenPipe (phost,
                           phost->Control.pipe_in,
                           0x80,
                           phost->device.address,
                           phost->device.speed,
                           USBH_EP_CONTROL,
                           phost->Control.pipe_size); 
      
      /* Open Control pipes */
      USBH_OpenPipe (phost,
                           phost->Control.pipe_out,
                           0x00,
                           phost->device.address,
                           phost->device.speed,
                           USBH_EP_CONTROL,
                           phost->Control.pipe_size);        
    }
    break;
    
  case ENUM_GET_CFG_DESC:  
    /* get standard configuration descriptor */
    if ( USBH_Get_CfgDesc(phost, 
                          USB_CONFIGURATION_DESC_SIZE) == USBH_OK)
    {
      phost->EnumState = ENUM_GET_FULL_CFG_DESC;        
    }
    break;
    
  case ENUM_GET_FULL_CFG_DESC:  
    /* get FULL config descriptor (config, interface, endpoints) */
    if (USBH_Get_CfgDesc(phost, 
                         phost->device.CfgDesc.wTotalLength) == USBH_OK)
    {
      phost->EnumState = ENUM_GET_MFC_STRING_DESC;       
    }
    break;
    
  case ENUM_GET_MFC_STRING_DESC:  
    if (phost->device.DevDesc.iManufacturer != 0)
    { /* Check that Manufacturer String is available */
      
      if ( USBH_Get_StringDesc(phost,
                               phost->device.DevDesc.iManufacturer, 
                                phost->device.Data , 
                               0xff) == USBH_OK)
      {
        /* User callback for Manufacturing string */
        USBH_UsrLog("Manufacturer : %s",  (char *)phost->device.Data);
        phost->EnumState = ENUM_GET_PRODUCT_STRING_DESC;
        
#if (USBH_USE_OS == 1)
    osMessagePut ( phost->os_event, USBH_STATE_CHANGED_EVENT, 0);
#endif          
      }
    }
    else
    {
     USBH_UsrLog("Manufacturer : N/A");      
     phost->EnumState = ENUM_GET_PRODUCT_STRING_DESC; 
#if (USBH_USE_OS == 1)
    osMessagePut ( phost->os_event, USBH_STATE_CHANGED_EVENT, 0);
#endif       
    }
    break;
    
  case ENUM_GET_PRODUCT_STRING_DESC:   
    if (phost->device.DevDesc.iProduct != 0)
    { /* Check that Product string is available */
      if ( USBH_Get_StringDesc(phost,
                               phost->device.DevDesc.iProduct, 
                               phost->device.Data, 
                               0xff) == USBH_OK)
      {
        /* User callback for Product string */
        USBH_UsrLog("Product : %s",  (char *)phost->device.Data);
        phost->EnumState = ENUM_GET_SERIALNUM_STRING_DESC;        
      }
    }
    else
    {
      USBH_UsrLog("Product : N/A");
      phost->EnumState = ENUM_GET_SERIALNUM_STRING_DESC; 
#if (USBH_USE_OS == 1)
    osMessagePut ( phost->os_event, USBH_STATE_CHANGED_EVENT, 0);
#endif        
    } 
    break;
    
  case ENUM_GET_SERIALNUM_STRING_DESC:   
    if (phost->device.DevDesc.iSerialNumber != 0)
    { /* Check that Serial number string is available */    
      if ( USBH_Get_StringDesc(phost,
                               phost->device.DevDesc.iSerialNumber, 
                               phost->device.Data, 
                               0xff) == USBH_OK)
      {
        /* User callback for Serial number string */
        USBH_UsrLog("Serial Number : %s",  (char *)phost->device.Data);
        Status = USBH_OK;
      }
    }
    else
    {
      USBH_UsrLog("Serial Number : N/A"); 
      Status = USBH_OK;
#if (USBH_USE_OS == 1)
    osMessagePut ( phost->os_event, USBH_STATE_CHANGED_EVENT, 0);
#endif        
    }  
    break;
    
  default:
    break;
  }  
  return Status;
}

/**
  * @brief  USBH_LL_SetTimer 
  *         Set the initial Host Timer tick
  * @param  phost: Host Handle
  * @retval None
  */
void  USBH_LL_SetTimer  (USBH_HandleTypeDef *phost, uint32_t time)
{
  phost->Timer = time;
}
/**
  * @brief  USBH_LL_IncTimer 
  *         Increment Host Timer tick
  * @param  phost: Host Handle
  * @retval None
  */
void  USBH_LL_IncTimer  (USBH_HandleTypeDef *phost)
{
  phost->Timer ++;
  USBH_HandleSof(phost);
}

/**
  * @brief  USBH_HandleSof 
  *         Call SOF process
  * @param  phost: Host Handle
  * @retval None
  */
void  USBH_HandleSof  (USBH_HandleTypeDef *phost)
{
  if((phost->gState == HOST_CLASS)&&(phost->pActiveClass != NULL))
  {
    phost->pActiveClass->SOFProcess(phost);
  }
}
/**
  * @brief  USBH_LL_Connect 
  *         Handle USB Host connexion event
  * @param  phost: Host Handle
  * @retval USBH_Status
  */
USBH_StatusTypeDef  USBH_LL_Connect  (USBH_HandleTypeDef *phost)
{
	USBH_EventTypeDef e;
	e.evt = USBH_EVT_CONNECT;
	e.timestamp = HAL_GetTick();
	USBH_PutEvent(e);
	return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_PortUp (USBH_HandleTypeDef *phost)
{
	USBH_EventTypeDef e;
	e.evt = USBH_EVT_PORTUP;
	e.timestamp = HAL_GetTick();
	USBH_PutEvent(e);
	return USBH_OK;
}

/**
  * @brief  USBH_LL_Disconnect 
  *         Handle USB Host disconnexion event
  * @param  phost: Host Handle
  * @retval USBH_Status
  */
USBH_StatusTypeDef  USBH_LL_Disconnect  (USBH_HandleTypeDef *phost)
{
	USBH_EventTypeDef e;
	e.evt = USBH_EVT_DISCONNECT;
	e.timestamp = HAL_GetTick();
	USBH_PutEvent(e);
	return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_PortDown (USBH_HandleTypeDef *phost)
{
	USBH_EventTypeDef e;
	e.evt = USBH_EVT_PORTDOWN;
	e.timestamp = HAL_GetTick();
	USBH_PutEvent(e);
	return USBH_OK;
}

/*********************************************************************/

USBH_StatusTypeDef USBH_LL_HCINT(USBH_HandleTypeDef *phost, struct hcint_t * hcint) {

  USBH_EventTypeDef e;
  e.evt = USBH_EVT_HCINT;
  e.timestamp = HAL_GetTick();
  e.data.hcint = *hcint;
  USBH_PutEvent(e);
  return USBH_OK;
}

/*****************************************************************************/
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

#if (USBH_USE_OS == 1)  
/**
  * @brief  USB Host Thread task
  * @param  pvParameters not used
  * @retval None
  */
static void USBH_Process_OS(void const * argument)
{
  osEvent event;
  
  for(;;)
  {
    event = osMessageGet(((USBH_HandleTypeDef *)argument)->os_event, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      USBH_Process((USBH_HandleTypeDef *)argument);
    }
   }
}

/**
* @brief  USBH_LL_NotifyURBChange 
*         Notify URB state Change
* @param  phost: Host handle
* @retval USBH Status
*/
USBH_StatusTypeDef  USBH_LL_NotifyURBChange (USBH_HandleTypeDef *phost)
{
  osMessagePut ( phost->os_event, USBH_URB_EVENT, 0);
  return USBH_OK;
}
#endif  
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
