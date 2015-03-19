#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"

#define PRINT_BUFFER_SIZE			65536

extern UART_HandleTypeDef 	huart2;

uint8_t print_buffer[PRINT_BUFFER_SIZE];
int in = 0;
int out = 0;
int printing = 0;

static int uart_printable(void);
static void uart_print(void);

void uart_ll_print(void) {
	if (printing)
		return;

	if (uart_printable()) {
		uart_print();
	}
}

void uart_hl_print(void) {

	if (uart_printable()) {

		printing = 1;

		if (uart_printable()) {
			uart_print();
		}

		printing = 0;
	}
}

/**
 *
 */
static int uart_printable(void) {

	if (in != out) {
		if (huart2.State == HAL_UART_STATE_READY || huart2.State == HAL_UART_STATE_BUSY_RX) {
			return 1;
		}
	}

	return 0;
}

static void uart_print(void) {

//	HAL_UART_StateTypeDef state = huart2.State;
//
//	if (state == HAL_UART_STATE_READY || state == HAL_UART_STATE_BUSY_RX)
//	{
//		if (in == out)
//			return;

		if (in < out) {

			HAL_UART_Transmit_DMA(&huart2, &print_buffer[out], PRINT_BUFFER_SIZE - out);
			out = 0;
		}
		else {

			HAL_UART_Transmit_DMA(&huart2, &print_buffer[out], in - out);
			out = in;
		}
//	}
}

int uart_write(int32_t file, uint8_t *ptr, int32_t len) {

    int i;

    for (i = 0; i < len; i++) {
    	print_buffer[in] = ptr[i];
    	in++;
    	if (in >= PRINT_BUFFER_SIZE) {
    		in = in - PRINT_BUFFER_SIZE;
    	}

    	if (in == out) {
    		return i;
    	};
    }

    uart_hl_print();
	return len;
}

void uart_print_tx_complete_cb() {

	uart_ll_print();
}

/*********************************************************/

#define RX_BUF_SIZE               (256) // must be 256
#define LINE_BUF_SIZE             (1024)
static uint8_t rx_buf[RX_BUF_SIZE];
static uint8_t rx_get = 0;

static uint8_t line_buf[LINE_BUF_SIZE];
static int line_buf_pos = 0;

extern void usb_host_log(void);

/*
 * Do it anyway, considering we cannot know what is the previously set buffer size, dont make assumptios,
 * leave the POLICY to the caller
 */
void Command_Init(void)
{
  rx_get = 0;
  HAL_UART_Receive_DMA(&huart2, rx_buf, RX_BUF_SIZE);
}

void Process_Command(void)
{
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

      if (0 == strcmp(line_buf, "p")) {
        usb_host_log();
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
}








