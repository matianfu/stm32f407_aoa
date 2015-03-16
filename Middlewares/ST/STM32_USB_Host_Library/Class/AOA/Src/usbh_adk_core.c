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
#include "usbh_adk_core.h"
#include "usart.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG

/** @defgroup USBH_ADK_CORE_Private_Variables
 * @{
 */
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN ADK_Machine_TypeDef ADK_Machine __ALIGN_END;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_Setup_TypeDef ADK_Setup __ALIGN_END;

/**
 * @}
 */

/** @defgroup USBH_ADK_CORE_Private_FunctionPrototypes
 * @{
 */
static USBH_StatusTypeDef USBH_AOA_InterfaceInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_InterfaceDeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_ADK_Handle(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_ClassRequest(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_GetProtocol(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_SendString(USBH_HandleTypeDef *phost,
    uint16_t index, uint8_t* buff);
static USBH_StatusTypeDef USBH_AOA_Switch(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_AOA_ConfigEndpoints(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_ADK_SOFProcess(USBH_HandleTypeDef *phost);

/*
 *
 */
USBH_ClassTypeDef USBH_ADK_cb =
{ "AOA",
USB_ADK_CLASS, USBH_AOA_InterfaceInit, USBH_AOA_InterfaceDeInit,
    USBH_AOA_ClassRequest, USBH_ADK_Handle, USBH_ADK_SOFProcess,
    NULL , };

/**  add by fan
 * @brief  USBH_ADK_SOFProcess
 *         The function is for SOF state
 * @param  phost: Host handle
 * @retval USBH Status
 */
static USBH_StatusTypeDef USBH_ADK_SOFProcess(USBH_HandleTypeDef *phost)
{
  return USBH_OK;
}
/**
 * @}
 */

/**
 * @brief  USBH_ADK_Init
 *         Initialization for ADK class.
 * @param  manufacture: manufacturer name string(max 63 chars)
 * @param  model: model name string (max 63 chars)
 * @param  description: description string (max 63 chars)
 * @param  version: version string (max 63 chars)
 * @param  uri: URI string (max 63 chars)
 * @param  serial: serial number string (max 63 chars)
 * @retval None
 */
void USBH_ADK_Init(uint8_t* manufacture, uint8_t* model, uint8_t* description,
    uint8_t* version, uint8_t* uri, uint8_t* serial)
{
  strncpy((char*)ADK_Machine.acc_manufacturer, (char*)manufacture, 64);
  ADK_Machine.acc_manufacturer[63] = '\0';
  strncpy((char*)ADK_Machine.acc_model, (char*)model, 64);
  ADK_Machine.acc_model[63] = '\0';
  strncpy((char*)ADK_Machine.acc_description, (char*)description, 64);
  ADK_Machine.acc_description[63] = '\0';
  strncpy((char*)ADK_Machine.acc_version, (char*)version, 64);
  ADK_Machine.acc_version[63] = '\0';
  strncpy((char*)ADK_Machine.acc_uri, (char*)uri, 64);
  ADK_Machine.acc_uri[63] = '\0';
  strncpy((char*)ADK_Machine.acc_serial, (char*)serial, 64);
  ADK_Machine.acc_serial[63] = '\0';

  ADK_Machine.initstate = ADK_INIT_SETUP;
  ADK_Machine.state = ADK_INITIALIZING; //ADK_ERROR;
}

/*
 * @brief   USBH_AOA_Handshake
 *          AOA handshake, if PID/VID does NOT match.
 * @param   USB host handle
 * @retval  USBH_FAIL if no disconnect occurs in specified time.
 */
USBH_StatusTypeDef USBH_AOA_Handshake(USBH_HandleTypeDef * phost)
{
  USBH_StatusTypeDef status;

  switch (ADK_Machine.initstate)
  {
  case ADK_INIT_SETUP:
    ADK_Machine.initstate = ADK_INIT_GET_PROTOCOL;
    ADK_Machine.protocol = -1;
    break;

  case ADK_INIT_GET_PROTOCOL:
    status = USBH_AOA_GetProtocol(phost);
    if (status == USBH_OK)
    {
      if (ADK_Machine.protocol >= 1)
      {
        ADK_Machine.initstate = ADK_INIT_SEND_MANUFACTURER;
        USBH_UsrLog("AOA: protocol version %d.", ADK_Machine.protocol);
      }
      else
      {
        ADK_Machine.initstate = ADK_INIT_FAILED;
        USBH_UsrLog("AOA: could not read device protocol version.");
      }
    }
    else if (status == USBH_BUSY) {
      // wait
    }
    else {
      USBH_UsrLog("AOA: get protocol command failed.")
      return USBH_FAIL;
    }
    break;

  case ADK_INIT_SEND_MANUFACTURER:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_MANUFACTURER,
        (uint8_t*) ADK_Machine.acc_manufacturer) == USBH_OK)
    {
      ADK_Machine.initstate = ADK_INIT_SEND_MODEL;
      USBH_UsrLog("AOA: SEND_MANUFACTURER %s", ADK_Machine.acc_manufacturer);
    }
    break;

  case ADK_INIT_SEND_MODEL:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_MODEL,
        (uint8_t*) ADK_Machine.acc_model) == USBH_OK)
    {
      ADK_Machine.initstate = ADK_INIT_SEND_DESCRIPTION;
      USBH_UsrLog("AOA: SEND_MODEL %s", ADK_Machine.acc_model);
    }
    break;

  case ADK_INIT_SEND_DESCRIPTION:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_DESCRIPTION,
        (uint8_t*) ADK_Machine.acc_description) == USBH_OK)
    {
      ADK_Machine.initstate = ADK_INIT_SEND_VERSION;
      USBH_UsrLog("AOA: SEND_DESCRIPTION %s", ADK_Machine.acc_description);
    }
    break;

  case ADK_INIT_SEND_VERSION:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_VERSION,
        (uint8_t*) ADK_Machine.acc_version) == USBH_OK)
    {
      ADK_Machine.initstate = ADK_INIT_SEND_URI;
      USBH_UsrLog("AOA: SEND_VERSION %s", ADK_Machine.acc_version);
    }
    break;

  case ADK_INIT_SEND_URI:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_URI,
        (uint8_t*) ADK_Machine.acc_uri) == USBH_OK)
    {
      ADK_Machine.initstate = ADK_INIT_SEND_SERIAL;
      USBH_UsrLog("AOA: SEND_URI %s", ADK_Machine.acc_uri);
    }
    break;

  case ADK_INIT_SEND_SERIAL:
    if (USBH_AOA_SendString(phost, ACCESSORY_STRING_SERIAL,
        (uint8_t*) ADK_Machine.acc_serial) == USBH_OK)
    {
      ADK_Machine.initstate = ADK_INIT_SWITCHING;
      // ADK_Machine.polling_timer = HAL_GetTick();
      USBH_UsrLog("AOA: SEND_SERIAL %s", ADK_Machine.acc_serial);
    }
    break;

  case ADK_INIT_SWITCHING:

    if (USBH_AOA_Switch(phost) == USBH_OK)
    {
      ADK_Machine.initstate = ADK_INIT_GET_DEVDESC;
      USBH_UsrLog("AOA: Switch to accessory mode");
    }
    break;

  default:
    break;
  }

  return USBH_BUSY;
}

/**
 * @brief  USBH_ADK_InterfaceInit
 *         Interface initialization for AOA class.
 *
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_StatusTypeDef : Status of class request handled.
 */
static USBH_StatusTypeDef USBH_AOA_InterfaceInit(USBH_HandleTypeDef * phost)
{
  uint8_t interface;

  USBH_UsrLog("AOA: Interface init.");
  /*
   * This is a strict match.
   */
  if (phost->device.DevDesc.idVendor == USB_ACCESSORY_VENDOR_ID
      && (phost->device.DevDesc.idProduct == USB_ACCESSORY_PRODUCT_ID
          || phost->device.DevDesc.idProduct == USB_ACCESSORY_ADB_PRODUCT_ID))
  {
    interface = USBH_FindInterface(phost, 0xFF, 0xFF, 0);
    if (interface == 0xFF) {
      USBH_UsrLog("AOA: Cannot find interface with Class 0xff, SubClass 0xff, Protocol 1");
      return USBH_FAIL;
    }


    if (USBH_SelectInterface(phost, interface) == USBH_FAIL) {
      return USBH_FAIL;
    }

    phost->pActiveClass->pData = &ADK_Machine;
    ADK_Machine.inSize = 0;
    ADK_Machine.outSize = 0;
    USBH_AOA_ConfigEndpoints(phost);    // this function configure in/out pipes.
    return USBH_OK;
  }
  else {
    USBH_UsrLog("AOA: Vendor id or Product id mismatch.");
    return USBH_FAIL;
  }
}

/**
 * @brief  USBH_ADK_InterfaceDeInit
 *         De-Initialize interface by freeing host channels allocated to interface
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval None
 */
USBH_StatusTypeDef USBH_AOA_InterfaceDeInit(USBH_HandleTypeDef *phost)
{
  USBH_UsrLog("AOA: Interface deinit.");

  /* close bulk transfer pipe */
  if (ADK_Machine.hc_num_out)
  {
    USBH_ClosePipe(phost, ADK_Machine.hc_num_out);
    USBH_FreePipe(phost, ADK_Machine.hc_num_out);

    ADK_Machine.hc_num_out = 0; /* Reset the Channel as Free */
  }

  if (ADK_Machine.hc_num_in)
  {

    USBH_ClosePipe(phost, ADK_Machine.hc_num_in);
    USBH_FreePipe(phost, ADK_Machine.hc_num_in);

    ADK_Machine.hc_num_in = 0; /* Reset the Channel as Free */
  }
  ADK_Machine.initstate = ADK_INIT_SETUP;

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
//  USBH_HandleTypeDef *pphost = phost;
//  USBH_StatusTypeDef status = USBH_BUSY;
//
//  switch (ADK_Machine.initstate)
//  {
//  case ADK_INIT_SETUP:
//    USBH_UsrLog("ADK");
//
//    // minimize NAK retry limit
//    // pdev->host.NakRetryLimit = USBH_ADK_NAK_RETRY_LIMIT;
//
//
//  case ADK_INIT_CONFIGURE_ANDROID:
//    USBH_ADK_ConfigEndpoints(phost);
//    ADK_Machine.initstate = ADK_INIT_DONE;
//    break;
//
//  case ADK_INIT_DONE:
//    status = USBH_OK;
//    ADK_Machine.state = ADK_IDLE;
//#ifdef DEBUG
//    printf("ADK:configuration complete.\r\n");
//#endif
//    break;
//
//  case ADK_INIT_FAILED:
//    status = USBH_UNRECOVERED_ERROR;
//    break;
//
//  default:
//    break;
//  }
//  return status;
  return USBH_OK;
}

/**
 * @brief  USBH_ADK_Handle
 *         ADK state machine handler
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_StatusTypeDef
 */
static USBH_StatusTypeDef USBH_ADK_Handle(USBH_HandleTypeDef *phost)
{

#if 0
  USBH_StatusTypeDef status = USBH_BUSY;

  switch (ADK_Machine.state)
  {
    case ADK_IDLE:
    ADK_Machine.state = ADK_SEND_DATA; //ADK_GET_DATA;//
    break;

    case ADK_SEND_DATA:

    if(ADK_Machine.outSize > 0)
    {
      USBH_BulkSendData(phost, ADK_Machine.outbuff, ADK_Machine.outSize, ADK_Machine.hc_num_out, 1);
      ADK_Machine.outSize = 0;
      //	ADK_Machine.state = ADK_GET_DATA;
    }

    ADK_Machine.state = ADK_GET_DATA;
    break;

    case ADK_GET_DATA:

    USBH_BulkReceiveData(phost, ADK_Machine.inbuff, USBH_ADK_DATA_SIZE, ADK_Machine.hc_num_in);
    ADK_Machine.state = ADK_IDLE;
    break;

    case ADK_BUSY:
    ADK_Machine.state = ADK_IDLE;
    ADK_Machine.outSize = 0;
    break;

    default:
    break;
  }
  status = USBH_OK;
  return status;
#else 
  return USBH_OK;
#endif
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
  phost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_VENDOR
      | USB_REQ_RECIPIENT_DEVICE;
  phost->Control.setup.b.bRequest = ACCESSORY_GET_PROTOCOL;
  phost->Control.setup.b.wValue.w = 0;
  phost->Control.setup.b.wIndex.w = 0;
  phost->Control.setup.b.wLength.w = 2;

  /* Control Request */
  return USBH_CtlReq(phost, (uint8_t*) &ADK_Machine.protocol, 2);
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
  USBH_UsrLog("AOA: Configure bulk endpoint.");

  if (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].bEndpointAddress & 0x80)
  {
    ADK_Machine.BulkInEp =
        (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].bEndpointAddress);
    ADK_Machine.BulkInEpSize =
        phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].wMaxPacketSize;

  }
  else
  {
    ADK_Machine.BulkOutEp =
        (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].bEndpointAddress);
    ADK_Machine.BulkOutEpSize =
        phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[0].wMaxPacketSize;
  }

  if (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].bEndpointAddress & 0x80)
  {
    ADK_Machine.BulkInEp =
        (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].bEndpointAddress);
    ADK_Machine.BulkInEpSize =
        phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].wMaxPacketSize;
  }
  else
  {
    ADK_Machine.BulkOutEp =
        (phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].bEndpointAddress);
    ADK_Machine.BulkOutEpSize =
        phost->device.CfgDesc.Itf_Desc[0].Ep_Desc[1].wMaxPacketSize;
  }

  ADK_Machine.hc_num_out = USBH_AllocPipe(phost, ADK_Machine.BulkOutEp);
  ADK_Machine.hc_num_in = USBH_AllocPipe(phost, ADK_Machine.BulkInEp);

  /* Open the new channels */
  USBH_OpenPipe(phost, ADK_Machine.hc_num_out, ADK_Machine.BulkOutEp,
      phost->device.address, phost->device.speed,
      EP_TYPE_BULK, ADK_Machine.BulkOutEpSize);

  USBH_OpenPipe(phost, ADK_Machine.hc_num_in, ADK_Machine.BulkInEp,
      phost->device.address, phost->device.speed,
      EP_TYPE_BULK, ADK_Machine.BulkInEpSize);

  return USBH_OK;
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
  memcpy(ADK_Machine.outbuff, buff, len);
  ADK_Machine.outSize = len;
  return USBH_OK;
}

USBH_StatusTypeDef USBH_ADK_send(USBH_HandleTypeDef *phost, uint8_t *buff,
    uint16_t len)
{
//	URB_STATE URB_Status;
//	HC_STATUS HCD_Status;
//	uint32_t HCD_GXferCnt;
//
//	URB_Status = HCD_GetURB_State(pdev , ADK_Machine.hc_num_out);
//	HCD_Status = HCD_GetHCState(pdev , ADK_Machine.hc_num_out);
//	HCD_GXferCnt = HCD_GetXferCnt(pdev , ADK_Machine.hc_num_out);

  if (len > 0)
  {
    USBH_BulkSendData(phost, buff, len, ADK_Machine.hc_num_out, 1);
//		ADK_Machine.outSize = 0;
    ADK_Machine.state = ADK_GET_DATA;	 //�޸�
  }
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
  xfercount = HCD_GetXferCnt(pdev, ADK_Machine.hc_num_in);
  if( xfercount > 0 )
  {
    memcpy(buff, ADK_Machine.inbuff, len);
    ADK_Machine.inSize = 0;
  }
  return (uint16_t)xfercount;
}

#else 
uint16_t USBH_ADK_read(USBH_HandleTypeDef *phost, uint8_t *buff, uint16_t len)
{
  uint32_t xfercount;
  uint32_t l = 0;

  xfercount = USBH_LL_GetLastXferSize(phost, ADK_Machine.hc_num_in); //     HCD_GetXferCnt(pdev, ADK_Machine.hc_num_in);
  if (xfercount >= len)
  {

    l = len;
    memcpy(buff, ADK_Machine.inbuff, len);
    ADK_Machine.inSize = 0;
  }
  else if (xfercount != 0)
  {
    l = xfercount;
    memcpy(buff, ADK_Machine.inbuff, xfercount);
    ADK_Machine.inSize = 0;
  }
  return (uint16_t) l;
}

#endif

//added by fan

//void USBH_ADK_ClearCount(USBH_HandleTypeDef *phost)
//{
//	HCD_ClearXferCnt(phost, ADK_Machine.hc_num_in);
//}

/**
 * @brief  USBH_ADK_getStatus
 *         Return ADK_Machine.state
 * @param  None
 * @retval ADK_Machine.state
 */
ADK_State USBH_ADK_getStatus(void)
{
  return ADK_Machine.state;
}
