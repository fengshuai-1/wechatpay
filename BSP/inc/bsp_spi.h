#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "bsp.h"

void spi_gpio_config(void);
void spi_config(void);
uint8_t SPI0_WriteByte(uint8_t Byte);

#endif
