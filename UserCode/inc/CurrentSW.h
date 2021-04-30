#ifndef	__CURRENTSW_H__
#define __CURRENTSW_H__

#include "bsp.h"
volatile extern uint8_t CHX;
extern volatile uint8_t CH_temp;
extern uint16_t point_y ;
extern uint16_t point_x ;
extern volatile uint16_t AVG_Cur;

void bsp_CurrentSW(void);
void Set_CH(uint8_t CH);
void SW_CTR(void);

uint8_t check_current(uint16_t *p);

#endif
