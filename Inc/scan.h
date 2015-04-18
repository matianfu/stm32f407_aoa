#ifndef __SCAN_H_
#define __SCAN_H_

#include "stm32f4xx_hal.h"
#define MAXIMUM_BARCODE_LENGTH 				(256)	/** CR/LF trailing included **/

typedef struct {

	uint16_t length;
	uint8_t code[MAXIMUM_BARCODE_LENGTH];

} barcode_t;

typedef enum{
	SCANNER_IDLE = 0,
	SCANNER_TIMEOUT,
	SCANNER_FAIL,
	SCANNER_BUSY,
	SCANNER_OK,
	SCANNER_ERROR

}Scanner_status_t;

typedef struct{
	Scanner_status_t status;
	barcode_t barcode;
	uint32_t timeout;
	//uint8_t code[MAXIMUM_BARCODE_LENGTH];
}scanner_infor_t;

extern scanner_infor_t scanner_infor;
void Scanner_Init(void);
void EnableScannerTrig(void);
void DisableScannerTig(void);
void Scanner_Handle(void);
void Scanner_Handle(void);

#endif





