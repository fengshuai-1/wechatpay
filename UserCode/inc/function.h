#ifndef __FUNCTION_H_
#define __FUNCTION_H_

#include "bsp.h"

void disPlayFunctionSelect(void); /* 显示功能选择界面 */
void disEnterPasswd(uint8_t passWordLength);/* 显示输入的密码长度 */
uint32_t getPassword(void);/* 读取密码 */
void setPasswd(void);/* 设置密码 */
void unlockByPasswd(void);/* 通过密码确认 */
void functionSelect10(void);/*功能选择*/
int Ordering(void);
void disPlayFunctionSelect1(void);


#endif

