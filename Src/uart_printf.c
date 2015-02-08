#include <stdint.h>
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef 	huart2;

uint8_t print_buffer[256];
uint8_t dma_buffer[256];
uint8_t in = 0;
uint8_t out = 0;

void uart_ll_print(void) {

	uint16_t count;
	HAL_UART_StateTypeDef state = huart2.State;

	if (state == HAL_UART_STATE_READY || state == HAL_UART_STATE_BUSY_RX)
	{
		if (in == out)
			return;

		if (in < out) {
			memmove(dma_buffer, &print_buffer[out], 256 - out);
			if (in)
			{
				memmove(&dma_buffer[256-out], print_buffer, in);
			}
			count = 256 - (out - in);
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

#if 0
	int n;
    switch (file) {
    case STDOUT_FILENO: /*stdout*/
        for (n = 0; n < len; n++) {
#if STDOUT_USART == 1
            while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART1->DR = (*ptr++ & (uint16_t)0x01FF);
#elif  STDOUT_USART == 2
            while ((USART2->SR & USART_FLAG_TC) == (uint16_t) RESET) {
            }
            USART2->DR = (*ptr++ & (uint16_t) 0x01FF);
#elif  STDOUT_USART == 3
            while ((USART3->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART3->DR = (*ptr++ & (uint16_t)0x01FF);
#endif
        }
        break;
    case STDERR_FILENO: /* stderr */
        for (n = 0; n < len; n++) {
#if STDERR_USART == 1
            while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART1->DR = (*ptr++ & (uint16_t)0x01FF);
#elif  STDERR_USART == 2
            while ((USART2->SR & USART_FLAG_TC) == (uint16_t) RESET) {
            }
            USART2->DR = (*ptr++ & (uint16_t) 0x01FF);
#elif  STDERR_USART == 3
            while ((USART3->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART3->DR = (*ptr++ & (uint16_t)0x01FF);
#endif
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
#endif

    int i;

    for (i = 0; i < len; i++) {
    	print_buffer[in++] = ptr[i];
    }
	return len;
}
