#ifndef __Flash_H__
#define __Flash_H__

#include "stm32f1xx_hal.h"
void Flash_write( uint32_t addr, uint32_t *data );
void Flash_read( uint32_t addr, uint32_t *data );



#endif

