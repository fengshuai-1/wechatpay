#ifndef __MENU_H__
#define __MENU_H__

#include "bsp.h"
extern int state;
extern uint32_t use_time;
void functionSelect(void);
void disPlayFunctionSelect(void);
void disPlayFunctionSelect1(void);
void disPlayFunctionSelect2(void);  //显示存件信息
void setPasswd(void);
uint32_t getPassword(void);
void disEnterPasswd(uint8_t passWordLength);
int Ordering(void);

void unlockByPasswd(void);
#endif
