#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "bsp.h"

extern uint8_t txbuffer[2000] ;
extern uint8_t rxbuffer[2000];
extern uint16_t tx_size ;
extern uint16_t rx_size ;

extern __IO uint16_t txcount ; 
extern __IO uint16_t rxcount; 

void uart0_init(void);
void Clear_Buffer(void);//Çå¿Õ»º´æ

#endif
