#include "scan.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define RX_BUF_SIZE               (256) // must be 256
static uint8_t rx_buf[RX_BUF_SIZE];
barcode_t barcode;



void Scanner_Init(void)
{
	HAL_UART_Receive_DMA(&huart2, rx_buf, RX_BUF_SIZE);
}

uint32_t Scanner_Handle(void)
{

	uint32_t Count ;
	UART_HandleTypeDef* h = &huart2;

	Count = __HAL_DMA_GET_COUNTER(h->hdmarx);
	rx_buf[Count] = " ";

	if(0 != strcmp("\r\n", rx_buf))
	{
		barcode.length = strlen(rx_buf) + 1 ;
		memcpy(barcode.code, rx_buf, strlen(rx_buf)+1);
	}

	return 1;
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



