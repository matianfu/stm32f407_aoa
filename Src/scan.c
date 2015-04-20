#include "scan.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "gpio.h"
#include "dma.h"
#include "stdbool.h"
#include "usbh_def.h"
#include "time.h"
#include "usbh_conf.h"


#define SCANNER_TRIG_PORT  GPIOC
#define SCANNER_TRIG_PIN   GPIO_PIN_8

#define SCANNER_BUFF_SIZE               (256) // must be 256
static unsigned char scan_buf[SCANNER_BUFF_SIZE];
scanner_infor_t scanner_infor;

static bool isDisplayableChar(uint8_t chr);
//static bool isLineFeed(uint8_t chr);
//static bool isCarriageReturn(uint8_t chr);


void Scanner_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  scanner_infor.status = SCANNER_IDLE;
  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();

  /*Configure GPIO pin : PC8 */
  GPIO_InitStruct.Pin = SCANNER_TRIG_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SCANNER_TRIG_PORT, &GPIO_InitStruct);
	HAL_UART_Receive_DMA(&huart3, scan_buf, SCANNER_BUFF_SIZE);
	DisableScannerTig();
	memset(scanner_infor.barcode.code, 0, MAXIMUM_BARCODE_LENGTH);
//	memcpy(scanner_infor.code, "NULL",4);
}

void EnableScannerTrig(void)
{
	HAL_GPIO_WritePin(SCANNER_TRIG_PORT, SCANNER_TRIG_PIN, GPIO_PIN_RESET);
}
void DisableScannerTig(void)
{
	HAL_GPIO_WritePin(SCANNER_TRIG_PORT, SCANNER_TRIG_PIN, GPIO_PIN_SET);
}

uint8_t CheckBarcode(scanner_infor_t *pScan)
{
	uint16_t i;

	if((pScan==NULL)|(pScan->barcode.length<=2))
		return 0;
	for(i=0; i< pScan->barcode.length-2; i++)
	{
		if(FALSE==isDisplayableChar(pScan->barcode.code[i]))
			return 0;
	}
	return 1;
}

void Scanner_Handle(void)
{

	uint32_t Count ;
	UART_HandleTypeDef* h = &huart3;

#if 1
	if(scanner_infor.status !=SCANNER_BUSY)
		return;
	if(scanner_infor.timeout>=3000)//3s
	{
		scanner_infor.status = SCANNER_TIMEOUT;
		DisableScannerTig();
		memset(scan_buf, 0, SCANNER_BUFF_SIZE);
		HAL_UART_Receive_DMA(&huart3, scan_buf, SCANNER_BUFF_SIZE);
	}
#endif
	Count = SCANNER_BUFF_SIZE - __HAL_DMA_GET_COUNTER(h->hdmarx);
	scan_buf[Count] = '\0';

	if((Count>2)&&(0 != strstr(scan_buf, "\r\n")))
	{
		DisableScannerTig();
		scanner_infor.barcode.length = strlen(scan_buf) ;
		memcpy(scanner_infor.barcode.code, scan_buf, strlen(scan_buf));
		memset(scan_buf, 0, SCANNER_BUFF_SIZE);

		/** suspend, disable tc intr before clear EN bit **/
		HAL_UART_DMAPause(&huart3);
		__HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_TC);
		__HAL_DMA_DISABLE(huart3.hdmarx);	/** this clear DMA_SxCR_EN bit**/

//		__HAL_DMA_SET_COUNTER(h->hdmarx, 256);
		HAL_UART_Receive_DMA(&huart3, scan_buf, SCANNER_BUFF_SIZE);


		/** clear tc flag and resume **/
		__HAL_DMA_CLEAR_FLAG(huart3.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(huart3.hdmarx));
		__HAL_DMA_ENABLE(huart3.hdmarx);
		__HAL_DMA_ENABLE_IT(huart3.hdmarx, DMA_IT_TC);
		HAL_UART_DMAResume(&huart3);
		if(CheckBarcode(&scanner_infor))
		{
			scanner_infor.status =SCANNER_OK;
			EnablePWM();
			HAL_Delay(100);
			DisablePWM();
		}
		else
		{
			scanner_infor.status =SCANNER_FAIL;
		}
	}
#if 0
	uint32_t ntdr;
  UART_HandleTypeDef* h = &huart2;
  uint8_t rx_put, ch;
  ntdr = __HAL_DMA_GET_COUNTER(h->hdmarx);

  rx_put = (uint8_t)(RX_BUF_SIZE - ntdr);
  while (rx_get != rx_put)
  {
    ch = rx_buf[rx_get];

    if (ch == '\r') {
      putchar('\r');
      putchar('\n');

      if (0 == strcmp("r", line_buf)){
        HAL_Delay(500);
        NVIC_SystemReset();
      }
      else if (0 == usb_host_cmd(line_buf)) {
        // usb_host_log();
      }
      else {
        printf("Unknown command: %s\r\n", line_buf);
      }

      memset(line_buf, 0, LINE_BUF_SIZE);
      line_buf_pos = 0;
    }
    else if (ch == '\b') {
      if (line_buf_pos) {
        line_buf_pos--;
        line_buf[line_buf_pos] = '\0';
        printf("\b \b");
      }
    }
    else {
      line_buf[line_buf_pos++] = ch;
      putchar(ch);
    }

    rx_get++;
  }
#endif
}


void restart_uart3_receive_dma(void)
{
	/** suspend, disable tc intr before clear EN bit **/
	HAL_UART_DMAPause(&huart3);
	__HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_TC);
	__HAL_DMA_DISABLE(huart3.hdmarx);	/** this clear DMA_SxCR_EN bit**/

//		__HAL_DMA_SET_COUNTER(h->hdmarx, 256);
	HAL_UART_Receive_DMA(&huart3, scan_buf, SCANNER_BUFF_SIZE);


	/** clear tc flag and resume **/
	__HAL_DMA_CLEAR_FLAG(huart3.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(huart3.hdmarx));
	__HAL_DMA_ENABLE(huart3.hdmarx);
	__HAL_DMA_ENABLE_IT(huart3.hdmarx, DMA_IT_TC);
	HAL_UART_DMAResume(&huart3);
}

/** tab is not included **/
static bool isDisplayableChar(uint8_t chr) {

	if (chr >= ' ' && chr <='~') {

		return TRUE;
	}

	return FALSE;
}
#if 0
static bool isLineFeed(uint8_t chr) {

	return (chr == 0x0A);
}

static bool isCarriageReturn(uint8_t chr) {

	return (chr == 0x0D);
}
#endif


