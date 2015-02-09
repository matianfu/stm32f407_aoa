#include <stdint.h>
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


