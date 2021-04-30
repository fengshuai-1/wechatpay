#ifndef __BSP_ADC_H
#define __BSP_ADC_H		
 
#include "bsp.h" 
 
extern volatile uint16_t    adc_value[20][2];

void dma_config(void);
void adc_config(void);

uint16_t Get_BAT(void);	 
uint16_t Get_LOAD_VCC(void);
void show_bat(void);

#endif 
