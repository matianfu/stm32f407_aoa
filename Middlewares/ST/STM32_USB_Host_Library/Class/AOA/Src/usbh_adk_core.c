/**
 ******************************************************************************
 * @file    usbh_adk_core.c
 * @author  Yuuichi Akagawa
 * @version V1.0.0
 * @date    2012/03/05
 * @brief   Android Open Accessory implementation
 ******************************************************************************
 * @attention
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * <h2><center>&copy; COPYRIGHT (C)2012 Yuuichi Akagawa</center></h2>
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usbh_adk_core.h"
#include "usart.h"
#include "usbh_core_helper.h"

/**
 * This functions are for AOA Class
 */
static USBH_StatusTypeDef USBH_AOA_InterfaceInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_InterfaceDeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_ClassRequest(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_Handle(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_SOFProcess(USBH_HandleTypeDef *phost);

/**
 * These functions are for AOA HandShake
 */
static USBH_StatusTypeDef USBH_AOA_GetProtocol(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_SendString(USBH_HandleTypeDef *phost,
    uint16_t index, uint8_t* buff);
static USBH_StatusTypeDef USBH_AOA_Switch(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_ConfigEndpoints(USBH_HandleTypeDef *phost);

static __ALIGN_BEGIN AOA_HandleTypeDef AOA_Handle_1 __ALIGN_END;
static __ALIGN_BEGIN AOA_HandleTypeDef AOA_Handle_2 __ALIGN_END;

USBH_ClassTypeDef USBH_AOA_CLASS_1 =
{
  .Name = "AOA",
  .ClassCode = USB_ADK_CLASS,
  .Init = USBH_AOA_InterfaceInit,
  .DeInit = USBH_AOA_InterfaceDeInit,
  .Requests = USBH_AOA_ClassRequest,
  .BgndProcess = USBH_AOA_Handle,
  .SOFProcess = USBH_AOA_SOFProcess,
  .pData = &AOA_Handle_1
};

USBH_ClassTypeDef USBH_AOA_CLASS_2 =
{
  .Name = "AOA",
  .ClassCode = USB_ADK_CLASS,
  .Init = USBH_AOA_InterfaceInit,
  .DeInit = USBH_AOA_InterfaceDeInit,
  .Requests = USBH_AOA_ClassRequest,
  .BgndProcess = USBH_AOA_Handle,
  .SOFProcess = USBH_AOA_SOFProcess,
  .pData = &AOA_Handle_2
};

/**
 * functions
 */
static bool printable(uint8_t* buf, uint32_t size)
{
  int i;

  for (i = 0; i < size; i++)
  {
    /**
     * ascii 0x20 (space) to 0x7E (tilde)
     */
    if (buf[i] < 0x20 || buf[i] > 0x7E)
    {
      return false;
    }
  }

  return true;
}

static char* bytes2bin(uint8_t* buf, uint32_t size) {

  static char out[256];
  static char* err = "ERROR";
  char* p;
  int i;

  if (buf == NULL || size == 0 || size > 64)
    return err;

  p = out;
  for (i = 0; i < size; i++)
  {
    sprintf(p, " %02x", buf[i]);
    p += 3;
  }
  *p = '\0';

  return out;
}

bool match_google_standard(USBH_HandleTypeDef *phost)
{
  if (phost->device.DevDesc.idVendor != USB_ACCESSORY_VENDOR_ID)
    return false;

  if (phost->device.DevDesc.idProduct == USB_ACCESSORY_PRODUCT_ID ||
      phost->device.DevDesc.idProduct == USB_ACCESSORY_ADB_PRODUCT_ID)
    return true;

  return false;
}

bool match_lenovo_yoga(USBH_HandleTypeDef *phost)
{
  if (phost->device.DevDesc.idVendor == 0x17ef &&
      phost->device.DevDesc.idProduct == 0x7799)
    return true;

  return false;
}

/**
 * @brief  USBH_AOA_InterfaceInit
 *         Interface initialization for AOA class.
 *
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_StatusTypeDef : Status of class request handled.
 */
static USBH_StatusTypeDef USBH_AOA_InterfaceInit(USBH_HandleTypeDef * phost)
{
  AOA_HandleTypeDef* aoa = phost->pActiveClass->pData;
  uint8_t interface;

  USBH_UsrLog("AOA: Interface init.");
  /*
   * This is a strict match.
   */
//  if (phost->device.DevDesc.idVendor == USB_ACCESSORY_VENDOR_ID
//      && (phost->device.DevDesc.idProduct == USB_ACCESSORY_PRODUCT_ID
//          || phost->device.DevDesc.idProduct == USB_ACCESSORY_ADB_PRODUCT_ID))
  if (match_google_standard(phost))
  {
    interface = USBH_FindInterface(phost, 0xFF, 0xFF, 0);
    if (interface == 0xFF) {
      USBH_UsrLog("AOA: Cannot find interface with Class 0xff, SubClass 0xff, Protocol 1");
      return USBH_FAIL;
    }

    if (USBH_SelectInterface(phost, interface) == USBH_FAIL) {
      return USBH_FAIL;
    }

    aoa->inState = AOA_RECV_DATA;
    aoa->outState = AOA_SEND_DATA;
    aoa->inSize = 0;
    aoa->outSize = 0;

    USBH_AOA_ConfigEndpoints(phost);    // this function configure in/out pipes.
    return USBH_OK;
  }
  else {
    USBH_UsrLog("AOA: Vendor id or Product id mismatch.");
    return USBH_FAIL;
  }
}

/**
 * @brief  USBH_AOA_InterfaceDeInit
 *         De-Initialize interface by freeing host channels allocated to interface
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval None
 */
static USBH_StatusTypeDef USBH_AOA_InterfaceDeInit(USBH_HandleTypeDef *phost)
{
  AOA_HandleTypeDef* aoa = phost->pActiveClass->pData;

  USBH_UsrLog("AOA: Interface deinit.");

  /* close bulk transfer pipe */
  if (aoa->hc_num_out)
  {
    USBH_ClosePipe(phost, aoa->hc_num_out);
    USBH_FreePipe(phost, aoa->hc_num_out);

    aoa->hc_num_out = 0; /* Reset the Channel as Free */
  }

  if (aoa->hc_num_in)
  {

    USBH_ClosePipe(phost, aoa->hc_num_in);
    USBH_FreePipe(phost, aoa->hc_num_in);

    aoa->hc_num_in = 0; /* Reset the Channel as Free */
  }

  return USBH_OK;
}

/**
 * @brief  USBH_ADK_ClassRequest
 *         This function will only initialize the ADK state machine
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_StatusTypeDef : Status of class request handled.
 */
static USBH_StatusTypeDef USBH_AOA_ClassRequest(USBH_HandleTypeDef *phost)
{
  return USBH_OK;
}

static USBH_StatusTypeDef USBH_AOA_InHandle(USBH_HandleTypeDef *phost)
{
  USBH_URBStateTypeDef URB_Status;
  uint32_t size;
  AOA_HandleTypeDef* aoa = phost->pActiveClass->pData;

  switch (aoa->inState)
  {
  case AOA_RECV_DATA:
    USBH_BulkReceiveData(phost, aoa->inbuff, USBH_ADK_DATA_SIZE,
        aoa->hc_num_in);

    aoa->inState = AOA_RECV_DATA_WAIT;
    break;

  case AOA_RECV_DATA_WAIT:
    URB_Status = USBH_LL_GetURBState(phost, aoa->hc_num_in);
    if (URB_Status == USBH_URB_DONE)
    {
      size = USBH_LL_GetLastXferSize(phost, aoa->hc_num_in);

      if (size > 0) {
        if (printable(aoa->inbuff, size)) {
          aoa->inbuff[size] = '\0';
          USBH_UsrLog("AOA: in %u bytes, \"%s\"", (unsigned int)size, aoa->inbuff);
        }
        else {
          USBH_UsrLog("AOA: in %u bytes, %s", (unsigned int)size, bytes2bin(aoa->inbuff, size));
        }
        memmove(aoa->outbuff, aoa->inbuff, size);
        aoa->outSize = size;
        aoa->inState = AOA_RECV_DATA;
      }
    }
    break;
  }

  return USBH_OK;
}

static USBH_StatusTypeDef USBH_AOA_OutHandle(USBH_HandleTypeDef *phost)
{
  USBH_URBStateTypeDef URB_Status;
  AOA_HandleTypeDef* aoa = phost->pActiveClass->pData;

  switch (aoa->outState)
  {
  case AOA_SEND_DATA:
    if (aoa->outSize > 0)
    {
      USBH_BulkSendData(phost, aoa->outbuff, aoa->outSize,
          aoa->hc_num_out, 0);
      aoa->outState = AOA_SEND_DATA_WAIT;
    }
    break;
  case AOA_SEND_DATA_WAIT:
    URB_Status = USBH_LL_GetURBState(phost, aoa->hc_num_out);
    switch (URB_Status)
    {
    case USBH_URB_DONE:
      aoa->outbuff[aoa->outSize] = '\0';
      USBH_UsrLog("AOA: out %d bytes, \"%s\"", aoa->outSize, aoa->outbuff)
      ;
      aoa->outSize = 0;
      aoa->outState = AOA_SEND_DATA;
      break;
    case USBH_URB_NOTREADY:

      aoa->outState = AOA_SEND_DATA;
      break;
    case USBH_URB_STALL:
      // TODO
      break;
    case USBH_URB_ERROR:
      // TODO
      break;
    default:
      break;
    }
    break;
  }

  return USBH_OK;
}

/**
 * @brief  USBH_ADK_Handle
 *         ADK state machine handler
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_StatusTypeDef
 */
static USBH_StatusTypeDef USBH_AOA_Handle(USBH_HandleTypeDef *phost)
{
  USBH_AOA_OutHandle(phost);
  USBH_AOA_InHandle(phost);
  return USBH_OK;
}

/**
 * @brief  USBH_AOA_SOFProcess
 *         The function is for SOF state
 * @param  phost: Host handle
 * @retval USBH Status
 */
static USBH_StatusTypeDef USBH_AOA_SOFProcess(USBH_HandleTypeDef *phost)
{
  return USBH_OK;
}

/**
 * @brief  USBH_ADK_getProtocol
 *         Inquiry protocol version number from Android device.
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_StatusTypeDef
 */
static USBH_StatusTypeDef USBH_AOA_GetProtocol(USBH_HandleTypeDef *phost)
{
  AOA_HandShakeDataTypeDef* p = phost->pUserData;

  phost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_VENDOR
      | USB_REQ_RECIPIENT_DEVICE;
  phost->Control.setup.b.bRequest = ACCESSORY_GET_PROTOCOL;
  phost->Control.setup.b.wValue.w = 0;
  phost->Control.setup.b.wIndex.w = 0;
  phost->Control.setup.b.wLength.w = 2;

  /* Control Request */
  return USBH_CtlReq(phost, (uint8_t*)&p->protocol, 2);
}

/**
 * @brief  USBH_AOA_SendString
 *         Send identifying string information to the Android device.
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @param  index: String ID
 * @param  buff: Identifying string
 * @retval USBH_StatusTypeDef
 */
static USBH_StatusTypeDef USBH_AOA_SendString(USBH_HandleTypeDef *phost,
    uint16_t index, uint8_t* buff)
{
  uint16_t length;
  length = (uint16_t) strlen((char*)buff) + 1;

  phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_VENDOR
      | USB_REQ_RECIPIENT_DEVICE;
  phost->Control.setup.b.bRequest = ACCESSORY_SEND_STRING;
  phost->Control.setup.b.wValue.w = 0;
  phost->Control.setup.b.wIndex.w = index;
  phost->Control.setup.b.wLength.w = length;

  /* Control Request */
  return USBH_CtlReq(phost, buff, length);
}

/**
 * @brief  USBH_ADK_switch
 *         Request the Android device start up in accessory mode.
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_StatusTypeDef
 */
static USBH_StatusTypeDef USBH_AOA_Switch(USBH_HandleTypeDef *phost)
{
  phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_VENDOR
      | USB_REQ_RECIPIENT_DEVICE;
  phost->Control.setup.b.bRequest = ACCESSORY_START;
  phost->Control.setup.b.wValue.w = 0;
  phost->Control.setup.b.wIndex.w = 0;
  phost->Control.setup.b.wLength.w = 0;

  /* Control Request */
  return USBH_CtlReq(phost, 0, 0);
}

/**
 * @brief  USBH_ADK_configAndroid
 *         Setup bulk transfer endpoint and open channel.
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_StatusTypeDef
 */
static USBH_StatusTypeDef USBH_AOA_ConfigEndpoints(USBH_HandleTypeDef * phost)
{
  AOA_HandleTypeDef* aoa = phost->pActiveClass->pData;
  USBH_UsrLog("AOA: Configure bulk endpoint.");

  if (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].bEndpointAddress & 0x80)
  {
    aoa->BulkInEp =
        (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].bEndpointAddress);
    aoa->BulkInEpSize =
        phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].wMaxPacketSize;

  }
  else
  {
    aoa->BulkOutEp =
        (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].bEndpointAddress);
    aoa->BulkOutEpSize =
        phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].wMaxPacketSize;
  }

  if (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].bEndpointAddress & 0x80)
  {
    aoa->BulkInEp =
        (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].bEndpointAddress);
    aoa->BulkInEpSize =
        phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].wMaxPacketSize;
  }
  else
  {
    aoa->BulkOutEp =
        (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].bEndpointAddress);
    aoa->BulkOutEpSize =
        phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].wMaxPacketSize;
  }

  aoa->hc_num_out = USBH_AllocPipe(phost, aoa->BulkOutEp);
  aoa->hc_num_in = USBH_AllocPipe(phost, aoa->BulkInEp);

  USBH_UsrLog("AOA: in pipe %d, out pipe %d", aoa->hc_num_in, aoa->hc_num_out);

  /* Open the new channels */
  USBH_OpenPipe(phost, aoa->hc_num_out, aoa->BulkOutEp,
      phost->device.address, phost->device.speed,
      EP_TYPE_BULK, aoa->BulkOutEpSize);

  USBH_OpenPipe(phost, aoa->hc_num_in, aoa->BulkInEp,
      phost->device.address, phost->device.speed,
      EP_TYPE_BULK, aoa->BulkInEpSize);

  return USBH_OK;
}

/*
 * @brief   USBH_AOA_Handshake
 *          AOA handshake, if PID/VID does NOT match.
 * @param   USB host handle
 * @retval  USBH_FAIL if no disconnect occurs in specified time.
 */
AOA_HandShakeResultTypeDef USBH_AOA_Handshake(USBH_HandleTypeDef * phost)
{
  USBH_StatusTypeDef status;
  AOA_HandShakeDataTypeDef* p = phost->pUserData;
  AOA_DeviceInfoTypeDef* deviceInfo;

  if (p == NULL)
  {
    return AOA_HANDSHAKE_ERROR;
  }

  deviceInfo = p->deviceInfo;

  switch (p->state)
  {
  case ADK_INIT_SETUP:
    p->state = ADK_INIT_GET_PROTOCOL;
    p->protocol = -1;
    break;

  case ADK_INIT_GET_PROTOCOL:
    status = USBH_AOA_GetProtocol(phost);
    if (status == USBH_BUSY)
    {
    }
    else if (status == USBH_OK)
    {
      if (p->protocol >= 1)
      {
        USBH_UsrLog("AOA: protocol version %d.", p->protocol);
        p->state = ADK_INIT_SEND_MANUFACTURER;
      }
      else
      {
        p->state = ADK_INIT_DONE;
        return AOA_HANDSHAKE_NOTSUPPORTED;
      }
    }
    else
    {
      p->state = ADK_INIT_DONE;
      return AOA_HANDSHAKE_ERROR;
    }
    break;

  case ADK_INIT_SEND_MANUFACTURER:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_MANUFACTURER,
        deviceInfo->acc_manufacturer) == USBH_OK)
    {
      p->state = ADK_INIT_SEND_MODEL;
      USBH_UsrLog("AOA: SEND_MANUFACTURER %s", deviceInfo->acc_manufacturer);
    }
    break;

  case ADK_INIT_SEND_MODEL:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_MODEL,
        deviceInfo->acc_model) == USBH_OK)
    {
      p->state = ADK_INIT_SEND_DESCRIPTION;
      USBH_UsrLog("AOA: SEND_MODEL %s", deviceInfo->acc_model);
    }
    break;

  case ADK_INIT_SEND_DESCRIPTION:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_DESCRIPTION,
        deviceInfo->acc_description) == USBH_OK)
    {
      p->state = ADK_INIT_SEND_VERSION;
      USBH_UsrLog("AOA: SEND_DESCRIPTION %s", deviceInfo->acc_description);
    }
    break;

  case ADK_INIT_SEND_VERSION:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_VERSION,
        deviceInfo->acc_version) == USBH_OK)
    {
      p->state = ADK_INIT_SEND_URI;
      USBH_UsrLog("AOA: SEND_VERSION %s", deviceInfo->acc_version);
    }
    break;

  case ADK_INIT_SEND_URI:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_URI, deviceInfo->acc_uri)
        == USBH_OK)
    {
      p->state = ADK_INIT_SEND_SERIAL;
      USBH_UsrLog("AOA: SEND_URI %s", deviceInfo->acc_uri);
    }
    break;

  case ADK_INIT_SEND_SERIAL:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_SERIAL,
        deviceInfo->acc_serial) == USBH_OK)
    {
      p->state = ADK_INIT_SWITCHING;
      USBH_UsrLog("AOA: SEND_SERIAL %s", deviceInfo->acc_serial);
    }
    break;

  case ADK_INIT_SWITCHING:
    if (USBH_AOA_Switch(phost) == USBH_OK)
    {
      p->state = ADK_INIT_DONE;
      USBH_UsrLog("AOA: Switch to accessory mode");
    }
    break;

  default:
    break;
  }

  return AOA_HANDSHAKE_BUSY;
}

/**
 * @brief  USBH_ADK_write
 *         Send data to Android device.
 * @param  pdev: Selected device
 * @param  buff: send data
 * @param  len : send data length
 * @retval USBH_StatusTypeDef
 */
USBH_StatusTypeDef USBH_ADK_write(USBH_HandleTypeDef *phost, uint8_t *buff,
    uint16_t len)
{
  AOA_HandleTypeDef* aoa = phost->pActiveClass->pData;
  memcpy(aoa->outbuff, buff, len);
  aoa->outSize = len;
  return USBH_OK;
}

USBH_StatusTypeDef USBH_ADK_send(USBH_HandleTypeDef *phost, uint8_t *buff,
    uint16_t len)
{
//	URB_STATE URB_Status;
//	HC_STATUS HCD_Status;
//	uint32_t HCD_GXferCnt;
//
//	URB_Status = HCD_GetURB_State(pdev , aoa->hc_num_out);
//	HCD_Status = HCD_GetHCState(pdev , aoa->hc_num_out);
//	HCD_GXferCnt = HCD_GetXferCnt(pdev , aoa->hc_num_out);

//  if (len > 0)
//  {
//    USBH_BulkSendData(phost, buff, len, aoa->hc_num_out, 1);
////		aoa->outSize = 0;
//    aoa->state = ADK_GET_DATA;
//  }
  return USBH_OK;
}

/**
 * @brief  USBH_ADK_read
 *         Receive data from  Android device.
 * @param  pdev: Selected device
 * @param  buff: receive data
 * @param  len : receive data buffer length
 * @retval received data length
 */
# if 0
uint16_t USBH_ADK_read(USB_OTG_CORE_HANDLE *pdev, uint8_t *buff, uint16_t len)
{
  uint32_t xfercount;
  xfercount = HCD_GetXferCnt(pdev, aoa->hc_num_in);
  if( xfercount > 0 )
  {
    memcpy(buff, aoa->inbuff, len);
    aoa->inSize = 0;
  }
  return (uint16_t)xfercount;
}

#else 
uint16_t USBH_ADK_read(USBH_HandleTypeDef *phost, uint8_t *buff, uint16_t len)
{
  AOA_HandleTypeDef* aoa = phost->pActiveClass->pData;
  uint32_t xfercount;
  uint32_t l = 0;

  xfercount = USBH_LL_GetLastXferSize(phost, aoa->hc_num_in); //     HCD_GetXferCnt(pdev, aoa->hc_num_in);
  if (xfercount >= len)
  {

    l = len;
    memcpy(buff, aoa->inbuff, len);
    aoa->inSize = 0;
  }
  else if (xfercount != 0)
  {
    l = xfercount;
    memcpy(buff, aoa->inbuff, xfercount);
    aoa->inSize = 0;
  }
  return (uint16_t) l;
}

#endif

//added by fan

//void USBH_ADK_ClearCount(USBH_HandleTypeDef *phost)
//{
//	HCD_ClearXferCnt(phost, aoa->hc_num_in);
//}

/**
 * @brief  USBH_ADK_getStatus
 *         Return aoa->state
 * @param  None
 * @retval aoa->state
 */
//ADK_State USBH_ADK_getStatus(void)
//{
//  return aoa->state;
//}
