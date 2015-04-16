#ifndef __SCAN_H_
#define __SCAN_H_

#define MAXIMUM_BARCODE_LENGTH 				(256)	/** CR/LF trailing included **/

typedef struct {

	uint16_t length;
	uint8_t code[MAXIMUM_BARCODE_LENGTH];

} barcode_t;

typedef struct{

};




#endif





