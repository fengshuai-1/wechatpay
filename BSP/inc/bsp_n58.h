#ifndef __BSP_N58_H
#define __BSP_N58_H

#include "bsp.h"
extern uint32_t total_fee;
void Make_xml(void);	   //���xml,��֧���ӿڷ���xml
void Make_xml1(void);	   //���xml,���ѯ�ӿڷ���xml

void Send_HTTPS(void);    // ��΢��֧���ӿڷ���xml
void Code_url(void);   //��ȡ��ά������


void Check_order(void);   //��ѯ����״̬
void trade_state(void);   //֧��״̬�ж�

#endif
