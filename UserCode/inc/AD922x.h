#ifndef	__AD922X_H__
#define __AD922X_H__

#include "bsp.h"

//#define AD_FIFOLEN   1024

extern unsigned short AD922x_data;
//extern volatile uint16_t AD_DataFIFO[AD_FIFOLEN];
//extern uint16_t AD_DataFIFO_LEN;

void AD9220_IO_Init(void);
unsigned short AD9220ReadData(void);
void Ctrl_10M_OUT(uint8_t ctrl);
unsigned short Get_AD9220(void);

#endif
