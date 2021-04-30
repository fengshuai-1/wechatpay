#ifndef __BSP_N58_H
#define __BSP_N58_H

#include "bsp.h"
extern uint32_t total_fee;
void Make_xml(void);	   //组合xml,向支付接口发送xml
void Make_xml1(void);	   //组合xml,向查询接口发送xml

void Send_HTTPS(void);    // 向微信支付接口发送xml
void Code_url(void);   //截取二维码链接


void Check_order(void);   //查询订单状态
void trade_state(void);   //支付状态判断

#endif
