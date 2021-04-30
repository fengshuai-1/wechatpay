#ifndef __BSP_DEBUG_H
#define __BSP_DEBUG_H

#include "bsp.h"

void Debug_Printf(char *format, ...);
void bsp_DebugInit(void);
void Debug_SendOneData(uint8_t buf);
void Debug_SendString(uint8_t* buf);
void Debug_SendData(uint8_t* buf,uint8_t len);


#endif


