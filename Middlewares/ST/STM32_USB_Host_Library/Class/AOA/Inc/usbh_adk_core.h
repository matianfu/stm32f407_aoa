/**
 ******************************************************************************
 * @file    usbh_adk_core.h
 * @author  Yuuichi Akagawa
 * @version V1.0.0
 * @date    2012/01/22
 * @brief   This file contains all the prototypes for the usbh_adk_core.c
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef USBH_ADK_CORE_H_
#define USBH_ADK_CORE_H_

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_conf.h"

/** @defgroup USBH_ADK_CORE_Exported_Defines
 * @{
 */
//AOA 1.0
#define USB_ACCESSORY_VENDOR_ID         	  0x18D1
#define USB_ACCESSORY_PRODUCT_ID        	  0x2D00
#define USB_ACCESSORY_ADB_PRODUCT_ID    	  0x2D01
//AOA 2.0
#define USB_AUDIO_PRODUCT_ID               	0x2D02
#define USB_AUDIO_ADB_PRODUCT_ID           	0x2D03
#define USB_ACCESSORY_AUDIO_PRODUCT_ID     	0x2D04
#define USB_ACCESSORY_AUDIO_ADB_PRODUCT_ID 	0x2D05

#define ACCESSORY_STRING_MANUFACTURER   	0
#define ACCESSORY_STRING_MODEL          	1
#define ACCESSORY_STRING_DESCRIPTION    	2
#define ACCESSORY_STRING_VERSION        	3
#define ACCESSORY_STRING_URI            	4
#define ACCESSORY_STRING_SERIAL         	5

//AOA 1.0
#define ACCESSORY_GET_PROTOCOL          	51
#define ACCESSORY_SEND_STRING           	52
#define ACCESSORY_START                 	53

//AOA 2.0
#define ACCESSORY_REGISTER_HID          	54
#define ACCESSORY_UNREGISTER_HID        	55
#define ACCESSORY_SET_HID_REPORT_DESC   	56
#define ACCESSORY_SEND_HID_EVENT        	57
#define ACCESSORY_SET_AUDIO_MODE        	58

#define USBH_ADK_DATA_SIZE					      64
#define USBH_ADK_NAK_RETRY_LIMIT 			    1

//added by fan

/*
 * This is interface class
 */
#define USB_ADK_CLASS                   	0xff
#define AOA_CODE							            0XFF
/**
 * @}
 */
/** @defgroup USBH_ADK_CORE_Exported_Types
 * @{
 */

extern USBH_ClassTypeDef      USBH_ADK_cb;
#define USBH_AOA_CLASS        &USBH_ADK_cb

typedef enum
{
  AOA_HANDSHAKE_BUSY = 0,
  AOA_HANDSHAKE_OK,
  AOA_HANDSHAKE_NOTSUPPORTED,
  AOA_HANDSHAKE_ERROR,
} AOA_HandShakeResultTypeDef;

typedef enum
{
  ADK_INIT_SETUP = 0,
  ADK_INIT_GET_PROTOCOL,
  ADK_INIT_SEND_MANUFACTURER,
  ADK_INIT_SEND_MODEL,
  ADK_INIT_SEND_DESCRIPTION,
  ADK_INIT_SEND_VERSION,
  ADK_INIT_SEND_URI,
  ADK_INIT_SEND_SERIAL,
  ADK_INIT_SWITCHING,
  ADK_INIT_DONE,
} AOA_HandShakeStateTypeDef;

typedef struct
{
  uint8_t acc_manufacturer[64];
  uint8_t acc_model[64];
  uint8_t acc_description[64];
  uint8_t acc_version[64];
  uint8_t acc_uri[64];
  uint8_t acc_serial[64];
} AOA_DeviceInfoTypeDef;

typedef struct {
  uint16_t protocol;
  AOA_DeviceInfoTypeDef * deviceInfo;
  AOA_HandShakeStateTypeDef state;
} AOA_HandShakeDataTypeDef;

typedef enum
{
  AOA_RECV_DATA = 0, AOA_RECV_DATA_WAIT,
} AOA_InStateTypeDef;

typedef enum
{
  AOA_SEND_DATA = 0, AOA_SEND_DATA_WAIT,
} AOA_OutStateTypeDef;

/* Structure for ADK process */
typedef struct
{
  uint8_t inbuff[USBH_ADK_DATA_SIZE];
  uint8_t outbuff[USBH_ADK_DATA_SIZE];

  uint16_t inSize;
  uint16_t outSize;

  AOA_InStateTypeDef inState;
  AOA_OutStateTypeDef outState;

  uint16_t pid;
  uint8_t hc_num_in;
  uint8_t hc_num_out;
  uint8_t BulkOutEp;
  uint8_t BulkInEp;
  uint16_t BulkInEpSize;
  uint16_t BulkOutEpSize;

} ADK_Machine_TypeDef;
/**
 * @}
 */

/** @defgroup USBH_ADK_CORE_Exported_FunctionsPrototype
 * @{
 */
void USBH_ADK_Init(uint8_t* manufacture, uint8_t* model, uint8_t* description,
    uint8_t* version, uint8_t* uri, uint8_t* serial);
USBH_StatusTypeDef USBH_ADK_write(USBH_HandleTypeDef *phost, uint8_t *buff,
    uint16_t len);
uint16_t USBH_ADK_read(USBH_HandleTypeDef *phost, uint8_t *buff, uint16_t len);
// ADK_State USBH_ADK_getStatus(void);

AOA_HandShakeResultTypeDef USBH_AOA_Handshake(USBH_HandleTypeDef * phost);

/**
 * @}
 */

#endif /* USBH_ADK_CORE_H_ */
