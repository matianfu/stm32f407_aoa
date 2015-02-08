#include <stdint.h>
#include "stm32f4xx_hal.h"

#define PRINT_BUFFER_SIZE			4096
#define PRINT_BUFFER_SIZE_MASK

extern UART_HandleTypeDef 	huart2;

uint8_t print_buffer[PRINT_BUFFER_SIZE];
uint8_t dma_buffer[PRINT_BUFFER_SIZE];
int in = 0;
int out = 0;
int printing = 0;

static void uart_print(void);

void uart_ll_print(void) {
	if (printing)
		return;

	uart_print();
}

void uart_hl_print(void) {
	printing = 1;
	uart_print();
	printing = 0;
}

static void uart_print(void) {

	uint16_t count;
	HAL_UART_StateTypeDef state = huart2.State;

	if (state == HAL_UART_STATE_READY || state == HAL_UART_STATE_BUSY_RX)
	{
		if (in == out)
			return;

		if (in < out) {
			memmove(dma_buffer, &print_buffer[out], PRINT_BUFFER_SIZE - out);
			if (in)
			{
				memmove(&dma_buffer[PRINT_BUFFER_SIZE-out], print_buffer, in);
			}
			count = PRINT_BUFFER_SIZE - (out - in);
		}
		else {
			memmove(dma_buffer, &print_buffer[out], in - out);
			count = in - out;
		}

		out = in;

	    HAL_UART_Transmit_DMA(&huart2, dma_buffer, count);
	}
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


