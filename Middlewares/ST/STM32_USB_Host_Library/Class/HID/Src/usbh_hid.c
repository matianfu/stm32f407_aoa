/**
 ******************************************************************************
 * @file    usbh_hid.c
 * @author  MCD Application Team
 * @version V3.1.0
 * @date    19-June-2014
 * @brief   This file is the HID Layer Handlers for USB Host HID class.
 *
 * @verbatim
 *
 *          ===================================================================
 *                                HID Class  Description
 *          ===================================================================
 *           This module manages the HID class V1.11 following the "Device Class Definition
 *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
 *           This driver implements the following aspects of the specification:
 *             - The Boot Interface Subclass
 *             - The Mouse and Keyboard protocols
 *
 *  @endverbatim
 *
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
#include "hid.h"
#include "usbh_hid.h"

/**
 * debug control
 */
static void suppress_in_pipe_debug_print(uint8_t pipenum)
{
	debug_hal_hcd_hc_submitrequest_print = 0;
	DebugConfig.channel_hcintx_mask[pipenum] = 0;
}

static void unsupress_in_pipe_debug_print(uint8_t pipenum)
{
  debug_hal_hcd_hc_submitrequest_print = DEBUG_HAL_HCD_HC_SUBMITREQUEST_DEFAULT;
  DebugConfig.channel_hcintx_mask[pipenum] = DebugConfig.channel_hcintx_default;
}

extern int hid_report_raw_event(struct hid_device *hid, int type, uint8_t *data, int size);

// extern int hid_device_probe(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USBHID_Probe(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_USBHID_Disconnect(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_HID_InterfaceInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_HID_InterfaceDeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_HID_ClassRequest(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_HID_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_HID_SOFProcess(USBH_HandleTypeDef *phost);
static void USBH_HID_ParseHIDDesc(HID_DescTypeDef *desc, uint8_t *buf);

extern USBH_StatusTypeDef USBH_HID_MouseInit(USBH_HandleTypeDef *phost);
extern USBH_StatusTypeDef USBH_HID_KeybdInit(USBH_HandleTypeDef *phost);
extern USBH_StatusTypeDef USBH_HID_NonBootKbdInit(USBH_HandleTypeDef *phost);

USBH_ClassTypeDef HID_Class =
{ "HID",
    USB_HID_CLASS, USBH_HID_InterfaceInit, USBH_HID_InterfaceDeInit,
    USBH_HID_ClassRequest, USBH_HID_Process, USBH_HID_SOFProcess,
    NULL };

/**
 * @brief  USBH_HID_InterfaceInit
 *         The function init the HID class.
 * @param  phost: Host handle
 * @retval USBH Status
 */
static USBH_StatusTypeDef USBH_HID_InterfaceInit(USBH_HandleTypeDef *phost)
{
  uint8_t max_ep;
  uint8_t num = 0;
  uint8_t interface;

  USBH_InterfaceDescTypeDef* itf_desc =  NULL;
  USBH_StatusTypeDef status = USBH_FAIL;
  HID_HandleTypeDef *HID_Handle;

  /*
   * see hid specification 1.11 (HID1_11.pdf) Ch 4.2
   *
   * for boot device, subclass code must be 1
   * and for keyboard and mouse, protocol code should be 1 and 2 respectively.
   * the original st hid class driver supports only boot device.
   *
   * for non-boot device, subclass code should be 0, according to Ch 4.3, this field (bInterfaceProtocol) should be 0.
   */

  /* find boot device first */
  interface = USBH_FindInterface(phost, phost->pActiveClass->ClassCode, HID_BOOT_CODE, 0xFF);

  if (interface == 0xFF) {
    /* find non-boot device second */
    // interface = USBH_FindInterface(phost, phost->pActiveClass->ClassCode, 0xFF, 0xFF);
    interface = USBH_FindInterface(phost, phost->pActiveClass->ClassCode, 0, 0);
  }

  if (interface == 0xFF) /* No Valid Interface */
  {
    status = USBH_FAIL;
    USBH_DbgLog("Cannot Find the interface for %s class.",
        phost->pActiveClass->Name);
  }
  else
  {
    USBH_SelectInterface(phost, interface);

    // Create HID_Handle for Class_Handle
    phost->pActiveClass->pData = (HID_HandleTypeDef *) USBH_malloc(sizeof(HID_HandleTypeDef));
    HID_Handle = phost->pActiveClass->pData;
    HID_Handle->state = HID_ERROR;

    /** init embedded hid_device **/
    /** this struct is initialized when class_request **/
    HID_Handle->hiddev = NULL;

    /*Decode Boot class Protocol: Mouse or Keyboard*/
    itf_desc = &phost->device.CfgDesc.Itf_Desc[phost->device.current_interface];

    if (itf_desc->bInterfaceSubClass == HID_BOOT_CODE) {
      if (phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bInterfaceProtocol
          == HID_KEYBRD_BOOT_CODE)
      {
        // Bootable HID Keyboard
        USBH_UsrLog("Bootable HID KeyBoard found!");
        HID_Handle->Init = USBH_HID_KeybdInit;
      }
      else if (phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bInterfaceProtocol
          == HID_MOUSE_BOOT_CODE)
      {
        USBH_UsrLog("Bootable HID Mouse found!");
        HID_Handle->Init = USBH_HID_MouseInit;
      }
      else
      {
        USBH_UsrLog("Bootable Device found but protocol not supported.");
        return USBH_FAIL;
      }
    }
    else {  // non-boot device, not sure if it is a keyboard
      USBH_UsrLog("Non-boot device found!");
      HID_Handle->Init = USBH_HID_NonBootKbdInit;
    }

    HID_Handle->state = HID_INIT;
    HID_Handle->ctl_state = HID_REQ_INIT;
    HID_Handle->ep_addr =
        phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[0].bEndpointAddress;
    HID_Handle->length =
        phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[0].wMaxPacketSize;
    HID_Handle->poll =
        phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[0].bInterval;

    if (HID_Handle->poll < HID_MIN_POLL)
    {
      HID_Handle->poll = HID_MIN_POLL;
    }

    /* Check fo available number of endpoints */
    /* Find the number of EPs in the Interface Descriptor */
    /* Choose the lower number in order not to overrun the buffer allocated */
    max_ep =
        ((phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bNumEndpoints
            <= USBH_MAX_NUM_ENDPOINTS) ?
            phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bNumEndpoints :
            USBH_MAX_NUM_ENDPOINTS);

    /* Decode endpoint IN and OUT address from interface descriptor */
    for (; num < max_ep; num++)
    {
      if (phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].bEndpointAddress
          & 0x80)
      {
        HID_Handle->InEp =
            (phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].bEndpointAddress);
        HID_Handle->InPipe = USBH_AllocPipe(phost, HID_Handle->InEp);

        /* Open pipe for IN endpoint */
        USBH_OpenPipe(phost, HID_Handle->InPipe, HID_Handle->InEp,
            phost->device.address, phost->device.speed,
            USB_EP_TYPE_INTR, HID_Handle->length);

        USBH_LL_SetToggle(phost, HID_Handle->InPipe, 0);

      }
      else
      {
        HID_Handle->OutEp =
            (phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].bEndpointAddress);
        HID_Handle->OutPipe = USBH_AllocPipe(phost, HID_Handle->OutEp);

        /* Open pipe for OUT endpoint */
        USBH_OpenPipe(phost, HID_Handle->OutPipe, HID_Handle->OutEp,
            phost->device.address, phost->device.speed,
            USB_EP_TYPE_INTR, HID_Handle->length);

        USBH_LL_SetToggle(phost, HID_Handle->OutPipe, 0);
      }

    }
    status = USBH_OK;
  }
  return status;
}

/**
 * @brief  USBH_HID_InterfaceDeInit
 *         The function DeInit the Pipes used for the HID class.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_HID_InterfaceDeInit(USBH_HandleTypeDef *phost)
{
  HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;

  USBH_UsrLog("%s", __func__);

  USBH_USBHID_Disconnect(phost);


  if (HID_Handle->InPipe != 0x00)
  {
    /*
     * restore debug default
     */
    unsupress_in_pipe_debug_print(HID_Handle->InPipe);

    USBH_ClosePipe(phost, HID_Handle->InPipe);
    USBH_FreePipe(phost, HID_Handle->InPipe);
    HID_Handle->InPipe = 0; /* Reset the pipe as Free */
  }

  if (HID_Handle->OutPipe != 0x00)
  {
    USBH_ClosePipe(phost, HID_Handle->OutPipe);
    USBH_FreePipe(phost, HID_Handle->OutPipe);
    HID_Handle->OutPipe = 0; /* Reset the pipe as Free */
  }

  if (phost->pActiveClass->pData)
  {
    HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef*)phost->pActiveClass->pData;

    if (HID_Handle) {
//  bug: this device is already freed in Disconnect
//      if (HID_Handle->hiddev) {
//        free(HID_Handle->hiddev);
//        HID_Handle->hiddev = NULL;
//      }

      // USBH_free(phost->pActiveClass->pData);
      free(HID_Handle);
      phost->pActiveClass->pData = NULL;
    }
  }

  debug_hal_hcd_hc_submitrequest_print = DEBUG_HAL_HCD_HC_SUBMITREQUEST_DEFAULT;

  return USBH_OK;
}

void USBH_HID_PrintHIDDesc(HID_DescTypeDef* desc) {

  USBH_UsrLog("  HID Device Descriptor:");
  USBH_UsrLog("    bLength: %d", desc->bLength);
  USBH_UsrLog("    bDescriptorType: %d", desc->bDescriptorType);
  USBH_UsrLog("    bcdHID: %d", desc->bcdHID);
  USBH_UsrLog("    bCountryCode: %d", desc->bCountryCode);
  USBH_UsrLog("    bNumDescriptors: %d", desc->bNumDescriptors);
  USBH_UsrLog("    bDescriptorType: %d %s", desc->bReportDescriptorType, "(for report descriptor)");
  USBH_UsrLog("    wDescriptorLength: %d %s", desc->wItemLength, "(for report descriptor)");
}



/**
 * @brief  USBH_HID_ClassRequest
 *         The function is responsible for handling Standard requests
 *         for HID class.
 * @param  phost: Host handle
 * @retval USBH Status
 */
static USBH_StatusTypeDef USBH_HID_ClassRequest(USBH_HandleTypeDef *phost)
{

  USBH_StatusTypeDef status = USBH_BUSY;
  USBH_StatusTypeDef classReqStatus = USBH_BUSY;
  HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;

  /* Switch HID state machine */
  switch (HID_Handle->ctl_state)
  {
  case HID_REQ_INIT:
  case HID_REQ_GET_HID_DESC:

    /* Get HID Desc */
    if (USBH_HID_GetHIDDescriptor(phost, USB_HID_DESC_SIZE) == USBH_OK)
    {
      USBH_HID_ParseHIDDesc(&HID_Handle->HID_Desc, phost->device.Data);
      // USBH_HID_PrintHIDDesc(&HID_Handle->HID_Desc);
      HID_Handle->ctl_state = HID_REQ_GET_REPORT_DESC;
    }

    break;
  case HID_REQ_GET_REPORT_DESC:

    /* Get Report Desc */
    if (USBH_HID_GetHIDReportDescriptor(phost, HID_Handle->HID_Desc.wItemLength) == USBH_OK)
    {
//      return USBH_FAIL; debug
//      HID_Handle->ctl_state = HID_REQ_SET_IDLE;
//      break;

      USBH_USBHID_Probe(phost);
      HID_Handle->ctl_state = HID_REQ_SET_IDLE;
      break;

//      if (USBH_USBHID_Probe(phost) == USBH_OK)
//      {
//        USBH_UsrLog("USBH_USBHID_Probe OK");
//        HID_Handle->ctl_state = HID_REQ_SET_IDLE;
//      }
//      else {
//        USBH_UsrLog("USBH_USBHID_Probe Fail");
//        return USBH_FAIL;
//      }
//      else
//      {
//        HID_Handle->ctl_state = HID_REQ_INIT;
//        return USBH_FAIL;
//      }
    }

    break;

  case HID_REQ_SET_IDLE:

    // return USBH_FAIL;
    classReqStatus = USBH_HID_SetIdle(phost, 0, 0);

    /* set Idle */
    if (classReqStatus == USBH_OK)
    {
      HID_Handle->ctl_state = HID_REQ_SET_PROTOCOL;
    }
    else if (classReqStatus == USBH_NOT_SUPPORTED)
    {
      HID_Handle->ctl_state = HID_REQ_SET_PROTOCOL;
    }
    break;

  case HID_REQ_SET_PROTOCOL:
    /* set protocol */
    if (USBH_HID_SetProtocol(phost, 0) == USBH_OK)
    {
      HID_Handle->ctl_state = HID_REQ_IDLE;

      /* suppress periodical debug print */
      suppress_in_pipe_debug_print(HID_Handle->InPipe);

      /* all requests performed*/
      phost->pUser(phost, HOST_USER_CLASS_ACTIVE);
      status = USBH_OK;
    }
    break;

  case HID_REQ_IDLE:
  default:
    break;
  }

  return status;
}

/**
 * @brief  USBH_HID_Process
 *         The function is for managing state machine for HID data transfers
 * @param  phost: Host handle
 * @retval USBH Status
 */

static int need_report(uint8_t* report, int size) {

  static int previous_zero = 1;
  int i;

  for (i = 0; i < size; i++) {
    if (report[i] != 0) {
      previous_zero = 0;
      return 1; // should report
    }
  }

  if (previous_zero != 1) {

    // USBH_UsrLog("Zero report");
    previous_zero = 1;
    return 1;
  }

  return 0;
}

static USBH_StatusTypeDef USBH_HID_Process(USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status = USBH_OK;
  HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;
  uint32_t xfer_count;

  switch (HID_Handle->state)
  {
  case HID_INIT:
    USBH_UsrLog("HID_INIT.");
    HID_Handle->Init(phost);
    HID_Handle->state = HID_IDLE;
    break;

  case HID_IDLE:
    if (USBH_HID_GetReport(phost, 0x01, 0, HID_Handle->pData,
        HID_Handle->length) == USBH_OK)
    {
      USBH_UsrLog("HID_GetReport return USBH_OK.")
      // fifo_write(&HID_Handle->fifo, HID_Handle->pData, HID_Handle->length);
      HID_Handle->state = HID_SYNC;
    }

    break;

  case HID_SYNC:

    /* Sync with start of Even Frame */
    if (phost->Timer & 1)
    {
      HID_Handle->state = HID_GET_DATA;
      USBH_UsrLog("SYNCed. Go to HID_GET_DATA.");
    }
#if (USBH_USE_OS == 1)
    osMessagePut ( phost->os_event, USBH_URB_EVENT, 0);
#endif   
    break;

  case HID_GET_DATA:

    USBH_InterruptReceiveData(phost, HID_Handle->pData, HID_Handle->length,
        HID_Handle->InPipe);

    HID_Handle->state = HID_POLL;
    HID_Handle->timer = phost->Timer;
    HID_Handle->DataReady = 0;
    // This prints every 10ms almost.
    // USBH_UsrLog("Urb submitted. Go to HID_POLL.");
    break;

  case HID_POLL:

    if (USBH_LL_GetURBState(phost, HID_Handle->InPipe) == USBH_URB_DONE)
    {
      if (HID_Handle->DataReady == 0)
      {

        // for USB rfid reader in Marlin's case, this prints every 30ms.
        // USBH_UsrLog("Urb done. Data is ready.");
        xfer_count = USBH_LL_GetLastXferSize(phost, HID_Handle->InPipe);

        if (need_report(HID_Handle->pData, xfer_count)) {
          USBH_UsrLog("in xfered bytes: %d", (int)xfer_count);
          // hid_input_report(&device, HID_INPUT_REPORT, HID_Handle->pData, xfer_count, 1);
          // hid_report_raw_event(&device, 0 /* HID_INPUT_REPORT */ , HID_Handle->pData, xfer_count);
          // TODO temporarily disable report processing
          hid_report_raw_event(HID_Handle->hiddev, HID_INPUT_REPORT, HID_Handle->pData, xfer_count);
        }

        // fifo_write(&HID_Handle->fifo, HID_Handle->pData, HID_Handle->length);
        HID_Handle->DataReady = 1;

        // USBH_UsrLog(":: 0x%02x 0x%02x", HID_Handle->pData[0], HID_Handle->pData[1]);
        // USBH_HID_EventCallback(phost);
#if (USBH_USE_OS == 1)
        osMessagePut ( phost->os_event, USBH_URB_EVENT, 0);
#endif          
      }
    }
    else if (USBH_LL_GetURBState(phost, HID_Handle->InPipe) == USBH_URB_STALL) /* IN Endpoint Stalled */
    {

      /* Issue Clear Feature on interrupt IN endpoint */
      if (USBH_ClrFeature(phost, HID_Handle->ep_addr) == USBH_OK)
      {
        /* Change state to issue next IN token */
        HID_Handle->state = HID_GET_DATA;
        USBH_UsrLog("Urb stall. Clear feature on EP. Go to GET_DATA.");
      }
    }

    break;

  default:
    break;
  }
  return status;
}

/**
 * @brief  USBH_HID_SOFProcess
 *         The function is for managing the SOF Process
 * @param  phost: Host handle
 * @retval USBH Status
 */
static USBH_StatusTypeDef USBH_HID_SOFProcess(USBH_HandleTypeDef *phost)
{
  HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;

  if (HID_Handle->state == HID_POLL)
  {
    if ((phost->Timer - HID_Handle->timer) >= HID_Handle->poll)
    {
      HID_Handle->state = HID_GET_DATA;
      // This prints once every 10ms, almost.
      // USBH_UsrLog("HID_POLLING time out. Go to GET_DATA.");
#if (USBH_USE_OS == 1)
      osMessagePut ( phost->os_event, USBH_URB_EVENT, 0);
#endif       
    }
  }
  return USBH_OK;
}

/**
 * @brief  USBH_Get_HID_ReportDescriptor
 *         Issue report Descriptor command to the device. Once the response
 *         received, parse the report descriptor and update the status.
 * @param  phost: Host handle
 * @param  Length : HID Report Descriptor Length
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_HID_GetHIDReportDescriptor(USBH_HandleTypeDef *phost,
    uint16_t length)
{

  USBH_StatusTypeDef status;

  status = USBH_GetDescriptor(phost,
  USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD,
  USB_DESC_HID_REPORT, phost->device.Data, length);

  /* HID report descriptor is available in phost->device.Data.
   In case of USB Boot Mode devices for In report handling ,
   HID report descriptor parsing is not required.
   In case, for supporting Non-Boot Protocol devices and output reports,
   user may parse the report descriptor*/

  return status;
}

/**
 * @brief  USBH_Get_HID_Descriptor
 *         Issue HID Descriptor command to the device. Once the response
 *         received, parse the report descriptor and update the status.
 * @param  phost: Host handle
 * @param  Length : HID Descriptor Length
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_HID_GetHIDDescriptor(USBH_HandleTypeDef *phost,
    uint16_t length)
{

  USBH_StatusTypeDef status;

  status = USBH_GetDescriptor(phost,
  USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD,
  USB_DESC_HID, phost->device.Data, length);

  return status;
}

/**
 * @brief  USBH_Set_Idle
 *         Set Idle State.
 * @param  phost: Host handle
 * @param  duration: Duration for HID Idle request
 * @param  reportId : Targetted report ID for Set Idle request
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_HID_SetIdle(USBH_HandleTypeDef *phost, uint8_t duration,
    uint8_t reportId)
{

  phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE
      |\
 USB_REQ_TYPE_CLASS;

  phost->Control.setup.b.bRequest = USB_HID_SET_IDLE;
  phost->Control.setup.b.wValue.w = (duration << 8) | reportId;

  phost->Control.setup.b.wIndex.w = 0;
  phost->Control.setup.b.wLength.w = 0;

  return USBH_CtlReq(phost, 0, 0);
}

/**
 * @brief  USBH_HID_Set_Report
 *         Issues Set Report
 * @param  phost: Host handle
 * @param  reportType  : Report type to be sent
 * @param  reportId    : Targetted report ID for Set Report request
 * @param  reportBuff  : Report Buffer
 * @param  reportLen   : Length of data report to be send
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_HID_SetReport(USBH_HandleTypeDef *phost,
    uint8_t reportType, uint8_t reportId, uint8_t* reportBuff,
    uint8_t reportLen)
{

  phost->Control.setup.b.bmRequestType = USB_H2D|USB_REQ_RECIPIENT_INTERFACE|USB_REQ_TYPE_CLASS;

  phost->Control.setup.b.bRequest = USB_HID_SET_REPORT;
  phost->Control.setup.b.wValue.w = (reportType << 8) | reportId;

  phost->Control.setup.b.wIndex.w = 0;
  phost->Control.setup.b.wLength.w = reportLen;

  return USBH_CtlReq(phost, reportBuff, reportLen);
}

/**
 * @brief  USBH_HID_GetReport
 *         retreive Set Report
 * @param  phost: Host handle
 * @param  reportType  : Report type to be sent
 * @param  reportId    : Targetted report ID for Set Report request
 * @param  reportBuff  : Report Buffer
 * @param  reportLen   : Length of data report to be send
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_HID_GetReport(USBH_HandleTypeDef *phost,
    uint8_t reportType, uint8_t reportId, uint8_t* reportBuff,
    uint8_t reportLen)
{
  phost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;

  phost->Control.setup.b.bRequest = USB_HID_GET_REPORT;
  phost->Control.setup.b.wValue.w = (reportType << 8) | reportId;

  phost->Control.setup.b.wIndex.w = 0;
  phost->Control.setup.b.wLength.w = reportLen;

  return USBH_CtlReq(phost, reportBuff, reportLen);
}

/**
 * @brief  USBH_Set_Protocol
 *         Set protocol State.
 * @param  phost: Host handle
 * @param  protocol : Set Protocol for HID : boot/report protocol
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_HID_SetProtocol(USBH_HandleTypeDef *phost,
    uint8_t protocol)
{

  phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;

  phost->Control.setup.b.bRequest = USB_HID_SET_PROTOCOL;
  phost->Control.setup.b.wValue.w = protocol != 0 ? 0 : 1;
  phost->Control.setup.b.wIndex.w = 0;
  phost->Control.setup.b.wLength.w = 0;

  return USBH_CtlReq(phost, 0, 0);

}

/**
 * @brief  USBH_ParseHIDDesc
 *         This function Parse the HID descriptor
 * @param  desc: HID Descriptor
 * @param  buf: Buffer where the source descriptor is available
 * @retval None
 */
static void USBH_HID_ParseHIDDesc(HID_DescTypeDef *desc, uint8_t *buf)
{

  desc->bLength = *(uint8_t *) (buf + 0);
  desc->bDescriptorType = *(uint8_t *) (buf + 1);
  desc->bcdHID = LE16(buf + 2);
  desc->bCountryCode = *(uint8_t *) (buf + 4);
  desc->bNumDescriptors = *(uint8_t *) (buf + 5);
  desc->bReportDescriptorType = *(uint8_t *) (buf + 6);
  desc->wItemLength = LE16(buf + 7);
}

// static void USBH_HID_ParseReportDesc()

/**
 * @brief  USBH_HID_GetDeviceType
 *         Return Device function.
 * @param  phost: Host handle
 * @retval HID function: HID_MOUSE / HID_KEYBOARD
 */
HID_TypeTypeDef USBH_HID_GetDeviceType(USBH_HandleTypeDef *phost)
{
  HID_TypeTypeDef type = HID_UNKNOWN;

  if (phost->gState == HOST_CLASS)
  {

    if (phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bInterfaceProtocol
        == HID_KEYBRD_BOOT_CODE)
    {
      type = HID_KEYBOARD;
    }
    else if (phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bInterfaceProtocol
        == HID_MOUSE_BOOT_CODE)
    {
      type = HID_MOUSE;
    }
  }
  return type;
}

/**
 * @brief  USBH_HID_GetPollInterval
 *         Return HID device poll time
 * @param  phost: Host handle
 * @retval poll time (ms)
 */
uint8_t USBH_HID_GetPollInterval(USBH_HandleTypeDef *phost)
{
  HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;

  if ((phost->gState == HOST_CLASS_REQUEST) || (phost->gState == HOST_INPUT)
      || (phost->gState == HOST_SET_CONFIGURATION)
      || (phost->gState == HOST_CHECK_CLASS) || ((phost->gState == HOST_CLASS)))
  {
    return (HID_Handle->poll);
  }
  else
  {
    return 0;
  }
}
/**
 * @brief  fifo_init
 *         Initialize FIFO.
 * @param  f: Fifo address
 * @param  buf: Fifo buffer
 * @param  size: Fifo Size
 * @retval none
 */
void fifo_init(FIFO_TypeDef * f, uint8_t * buf, uint16_t size)
{
  f->head = 0;
  f->tail = 0;
  f->lock = 0;
  f->size = size;
  f->buf = buf;
}

/**
 * @brief  fifo_read
 *         Read from FIFO.
 * @param  f: Fifo address
 * @param  buf: read buffer
 * @param  nbytes: number of item to read
 * @retval number of read items
 */
uint16_t fifo_read(FIFO_TypeDef * f, void * buf, uint16_t nbytes)
{
  uint16_t i;
  uint8_t * p;
  p = buf;

  if (f->lock == 0)
  {
    f->lock = 1;
    for (i = 0; i < nbytes; i++)
    {
      if (f->tail != f->head)
      {
        *p++ = f->buf[f->tail];
        f->tail++;
        if (f->tail == f->size)
        {
          f->tail = 0;
        }
      }
      else
      {
        f->lock = 0;
        return i;
      }
    }
  }
  f->lock = 0;
  return nbytes;
}

/**
 * @brief  fifo_write
 *         Read from FIFO.
 * @param  f: Fifo address
 * @param  buf: read buffer
 * @param  nbytes: number of item to write
 * @retval number of written items
 */
uint16_t fifo_write(FIFO_TypeDef * f, const void * buf, uint16_t nbytes)
{
  uint16_t i;
  const uint8_t * p;
  p = buf;
  if (f->lock == 0)
  {
    f->lock = 1;
    for (i = 0; i < nbytes; i++)
    {
      if ((f->head + 1 == f->tail)
          || ((f->head + 1 == f->size) && (f->tail == 0)))
      {
        f->lock = 0;
        return i;
      }
      else
      {
        f->buf[f->head] = *p++;
        f->head++;
        if (f->head == f->size)
        {
          f->head = 0;
        }
      }
    }
  }
  f->lock = 0;
  return nbytes;
}

/**
 * @brief  The function is a callback about HID Data events
 * @param  phost: Selected device
 * @retval None
 */


__weak void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
  HID_KEYBD_Info_TypeDef* kbinfo;

  static HID_KEYBD_Info_TypeDef prev = {0};

  if (NULL != (kbinfo = USBH_HID_GetKeybdInfo(phost)))
  {
    // bypass successive
    if (0 == memcmp(kbinfo, &prev, sizeof(prev)))
      return;

    USBH_UsrLog(
        "modifiers: lctrl %d rctrl %d lshift %d rshift %d lalt %d ralt %d lgui %d rgui %d",
        kbinfo->lctrl, kbinfo->rctrl, kbinfo->lshift, kbinfo->rshift,
        kbinfo->lalt, kbinfo->ralt, kbinfo->lgui, kbinfo->rgui);

    USBH_UsrLog("keycodes: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x",
        kbinfo->keys[0], kbinfo->keys[1], kbinfo->keys[2], kbinfo->keys[3],
        kbinfo->keys[4], kbinfo->keys[5]);

    prev = *kbinfo;
  }
}

/************************ Custom Functions ************************************/

void USBH_Print_Report_Descriptor(uint8_t* buf, uint16_t size)
{
  unsigned i;

  /* print raw data of report descriptor */
  USBH_UsrLog("HID: Print raw data of HID report descriptor.");

  // TODO print thru option
  for (i = 0; i < size; i++)
  {
    USBH_UsrLog(" - 0x%02x", buf[i]);
  }
}

void USBH_Print_HID_Object_Size(void) {

  USBH_UsrLog("HID: size of hid_device is %d", sizeof(struct hid_device));
  USBH_UsrLog("HID: size of hid_parser is %d", sizeof(struct hid_parser));
}

/**
 * @brief   HID Device Probe
 *          Assuming the report descriptor is available in phost->device.Data
 *
 *          This function merges logic in both usbhid_probe and hid_probe.
 * @param   phost
 * @retval  status
 */
static USBH_StatusTypeDef USBH_USBHID_Probe(USBH_HandleTypeDef *phost)
{
  int ret = 0;
  struct hid_device* hiddev;
  HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;

  /* The descriptor is available in phost->device.Data */
  uint8_t *rdesc = phost->device.Data;
  uint16_t rsize = HID_Handle->HID_Desc.wItemLength;

  // USBH_Print_Report_Descriptor(rdesc, rsize);
  // USBH_Print_HID_Object_Size();

  hiddev = hid_request_device();

  if (hiddev == NULL)
    goto fail;

  ret = hid_set_report_descriptor(hiddev, rdesc, rsize);
  if (ret)
    goto fail;

  ret = hid_open_report(hiddev);
  if (ret)
    goto fail;

  /* ret = hid_connect(hiddev, HID_CONNECT_DEFAULT); */
  ret = hidinput_connect(hiddev, 0);    // force?
  if (ret)
    goto fail;

  hiddev->claimed |= HID_CLAIMED_INPUT;

  HID_Handle->hiddev = hiddev;
  return USBH_OK;

fail: hid_release_device(hiddev);

  USBH_UsrLog("Hid probe fail");
  return USBH_FAIL;
}

static USBH_StatusTypeDef USBH_USBHID_Disconnect(USBH_HandleTypeDef *phost) {

  HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;
  struct hid_device *hiddev = HID_Handle->hiddev;

  if (hiddev) {
    hid_release_device(hiddev);
    hiddev = NULL;
  }

  return USBH_OK;
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
