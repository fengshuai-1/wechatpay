#ifndef __FUNCTION_H_
#define __FUNCTION_H_

#include "bsp.h"

void disPlayFunctionSelect(void); /* ��ʾ����ѡ����� */
void disEnterPasswd(uint8_t passWordLength);/* ��ʾ��������볤�� */
uint32_t getPassword(void);/* ��ȡ���� */
void setPasswd(void);/* �������� */
void unlockByPasswd(void);/* ͨ������ȷ�� */
void functionSelect10(void);/*����ѡ��*/
int Ordering(void);
void disPlayFunctionSelect1(void);


#endif

