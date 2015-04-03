/**
 ******************************************************************************
  * @file            : USB_HOST
  * @date            : 04/02/2015 16:50:32 
  * @version         : v1.0_Cube
  * @brief           :  This file implements the USB Host 
  ******************************************************************************
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "usbh_hid.h"
#include "usbh_adk_core.h"

typedef enum {
  ABORT_HANDLE_INIT,
  ABORT_HANDLE_HANDSHAKE,
  ABORT_HANDLE_HANDSHAKE_DONE,
} AbortStateTypeDef;

/* USB Host Core handle declaration */
USBH_HandleTypeDef hUsbHostHS;
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;
AbortStateTypeDef  Abort_state = ABORT_HANDLE_INIT;

static int aoa_handshake_tried = 0;

static AOA_DeviceInfoTypeDef deviceInfo __attribute__ ((aligned(4)))
    =
{
  .acc_manufacturer = "Actnova",
  .acc_model = "DemoKit",
  .acc_description = "Android Open Accessory DemoKit",
  .acc_version = "1.0",
  .acc_uri = "http://developer.android.com/tools/adk/index.html",
  .acc_serial = "BL100A"
};

static AOA_HandShakeDataTypeDef handshakeData =
{
  .deviceInfo = &deviceInfo,
};

extern USBH_StatusTypeDef USBH_AOA_Handshake(USBH_HandleTypeDef * phost);

/**
* -- Insert your variables declaration here --
*/ 
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
* user callbak declaration
*/ 
static void USBH_UserProcess1  (USBH_HandleTypeDef *phost, uint8_t id);
// static void USBH_UserProcess2  (USBH_HandleTypeDef *phost, uint8_t id);

/**
* -- Insert your external function declaration here --
*/ 
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* init function */				        
void MX_USB_HOST_Init(void)
{
  /* init adk */
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
//  USBH_ADK_Init((unsigned char*)"Actnova",
//                (unsigned char*)"DemoKit",
//                (unsigned char*)"HID barcode scanner adapter",
//                (unsigned char*)"1.0",
//                (unsigned char*)"http://www.actnova.com/aoa.apk",
//                (unsigned char*)"1234567890");

  /* Init Host Library,Add Supported Class and Start the library*/
  USBH_Init(&hUsbHostHS, USBH_UserProcess1, HOST_HS);

  // USBH_RegisterClass(&hUsbHostHS, USBH_MSC_CLASS);
  USBH_RegisterClass(&hUsbHostHS, USBH_HID_CLASS);
  USBH_RegisterClass(&hUsbHostHS, USBH_AOA_CLASS);

  USBH_Start(&hUsbHostHS);

  /* Init Host Library,Add Supported Class and Start the library*/
//  USBH_Init(&hUsbHostFS, USBH_UserProcess2, HOST_FS);

//  USBH_RegisterClass(&hUsbHostFS, USBH_HID_CLASS);

//  USBH_Start(&hUsbHostFS);

}
/*
 * Background task
*/ 
void MX_USB_HOST_Process() 
{
  /* USB Host Background task */
    USBH_ProcessEvent(&hUsbHostHS);
    // USBH_Process(&hUsbHostFS);
}

/*
 * user callback definition
*/ 



static void USBH_UserProcess1(USBH_HandleTypeDef *phost, uint8_t id)
{
  AOA_HandShakeResultTypeDef r;
  /* USER CODE BEGIN 2 */
  switch (id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_DISCONNECT;
    break;

  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_READY;
    break;

  case HOST_USER_CONNECTION:
    Appli_state = APPLICATION_START;

    Abort_state = ABORT_HANDLE_INIT;
    aoa_handshake_tried = 0;
    break;

  case HOST_USER_HANDLE_ABORT:

    switch (Abort_state)
    {
    case ABORT_HANDLE_INIT:

      if (phost->AbortReason == ABORT_CLASSINIT_FAIL || /** interface with class code 0xff but subclass protocol not match **/
      phost->AbortReason == ABORT_NOCLASS_MATCH) /** no interface with class code 0xff but aoa may works **/
      {
        handshakeData.state = ADK_INIT_SETUP;
        handshakeData.protocol = -1;
        phost->pUserData = &handshakeData;

        Abort_state = ABORT_HANDLE_HANDSHAKE;
      }
      break;

    case ABORT_HANDLE_HANDSHAKE:

      r = USBH_AOA_Handshake(phost);
      switch (r)
      {
      case AOA_HANDSHAKE_OK:
        USBH_UsrLog("AOA Handshake OK, Waiting for re-connect.")
        ;
        Abort_state = ABORT_HANDLE_HANDSHAKE_DONE;
        break;
      case AOA_HANDSHAKE_NOTSUPPORTED:
        USBH_UsrLog("AOA Handshake fail, not supported")
        ;
        Abort_state = ABORT_HANDLE_HANDSHAKE_DONE;
        break;
      case AOA_HANDSHAKE_ERROR:
        USBH_UsrLog("AOA Handshake fail, error")
        ;
        Abort_state = ABORT_HANDLE_HANDSHAKE_DONE;
        break;

      default:
        break;
      }
      break;

    case ABORT_HANDLE_HANDSHAKE_DONE:
      break;
    }

    break;

  default:
    break;
  }
  /* USER CODE END 2 */
}

#if 0
static void USBH_UserProcess2  (USBH_HandleTypeDef *phost, uint8_t id)
{

  /* USER CODE BEGIN 2 */
  switch(id)
  { 
  case HOST_USER_SELECT_CONFIGURATION:
  break;
    
  case HOST_USER_DISCONNECTION:
  Appli_state = APPLICATION_DISCONNECT;
  break;
    
  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
  break;

  default:
  break; 
  }
  /* USER CODE END 2 */
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
