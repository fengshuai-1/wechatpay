#include "bsp.h"


char *strx;
uint16_t Rxcouter;
uint8_t txbuffer[2000];
uint8_t pow_flg;
uint16_t tx_size,rx_size;
uint8_t rxbuffer[2000];
__IO uint16_t txcount = 0; 
__IO uint16_t rxcount = 0; 


void N58_init(void)
{
	Clear_Buffer();   //清缓存
	AT_check();       //检查AT指令
	bsp_DelayMS(200);
	CSQ_check();      //信号强度检测
	bsp_DelayMS(200);
	IP_allo();        //分配IP
	bsp_DelayMS(200);
	HTTPS_cfg(); //直连不加密模式
	bsp_DelayMS(200);
//	HTTPS_para();//连接微信支付端口
//	bsp_DelayMS(200);	
}

/********************************************************

	清空Buffer中的数据

*********************************************************/

void Clear_Buffer(void)//清空缓存
{
    uint16_t i;
    for(i=0;i<Rxcouter;i++)
    {
			rxbuffer[i]=0;//缓存
		}
  Rxcouter=0;
}



/********************************************************

	清空txbuffer中的数据

*********************************************************/

void Clear_txbuffer(void)//清空txbuffer缓存
{
    uint16_t i;
    for(i=0;i<txcount;i++)
    {
			txbuffer[i]=0;//缓存
		}
  txcount=0;
}
/*******************************************************

	检测模块AT指令状态，回复OK可以正常发送AT指令，结束后
	清除buffer。
	对应AT指令：AT

*******************************************************/

void AT_check(void)
{
	u8 txbuffer1[]="AT\r\n";
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//返回OK
	/*等待返回OK*/
	while(strx==NULL)
	{
		Clear_Buffer();	
		u8 txbuffer2[]="AT\r\n";
		tx_size = sizeof(txbuffer2);
		memcpy(txbuffer,txbuffer2,tx_size);
		usart_interrupt_enable(USART0, USART_INT_TBE);		
		bsp_DelayMS(500);
		strx=strstr((const char*)rxbuffer,(const char*)"OK");//返回OK
	}
	Clear_Buffer();
}
/***************************************************

	检查模块信号值，如果信号值太差可能会导致联网失败
	对应AT指令：AT+CSQ?

***************************************************/

void CSQ_check(void)
{
	u8 txbuffer1[]="AT+CSQ\r\n";//检查CSQ
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	Clear_Buffer();	
	bsp_DelayMS(500);
	
}
/*****************************************************

	请求运营商分配IP
	对应AT指令：AT+XIIC=1 
	运营商不同本条指令有区别

******************************************************/

void IP_allo(void)
{
	u8 txbuffer1[]="AT+XIIC?\r\n";//检查是否分配IP
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"+XIIC:1");//判断结果有+XIIC:    1才能做下一步。
	while(strx==NULL)
	{
		Clear_Buffer();
		u8 txbuffer2[]="AT+XIIC=1\r\n";//分配IP
		tx_size = sizeof(txbuffer2);
		memcpy(txbuffer,txbuffer2,tx_size);
		usart_interrupt_enable(USART0, USART_INT_TBE);
		bsp_DelayMS(500);
		strx=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。
	}
	Clear_Buffer();
}


/*****************************************************

	选择直连不加密模式

******************************************************/

void HTTPS_cfg(void) //直连不加密模式
{
	Clear_Buffer();
	u8 txbuffer1[]="AT+HTTPSCFG =\"sslversion\",3\r\n"; //ssl 协议版本  3:TLS1.2
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。
	Clear_Buffer();
	
	u8 txbuffer2[]="AT+HTTPSCFG=\"authmode\",0\r\n"; //选择不认证直连模式
	tx_size = sizeof(txbuffer2);
	memcpy(txbuffer,txbuffer2,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。

}

/*****************************************************

连接微信支付端口

******************************************************/

void HTTPS_para(void)   //连接微信支付端口
{
	Clear_Buffer();
	u8 txbuffer1[]="AT+HTTPSPARA=url,api.mch.weixin.qq.com/pay/unifiedorder\r\n"; //连接微信支付接口

	
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。
	Clear_Buffer();
	
	u8 txbuffer2[]="AT+HTTPSPARA=port,443\r\n"; //端口号
	tx_size = sizeof(txbuffer2);
	memcpy(txbuffer,txbuffer2,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。


}

/*****************************************************

连接微信查询端口

******************************************************/

void HTTPS_check(void)   //连接微信查询端口
{
	Clear_Buffer();
	u8 txbuffer1[]="AT+HTTPSPARA=url,api.mch.weixin.qq.com/pay/orderquery\r\n"; //连接微信查询接口
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。
	Clear_Buffer();
	
	u8 txbuffer2[]="AT+HTTPSPARA=port,443\r\n"; //端口号
	tx_size = sizeof(txbuffer2);
	memcpy(txbuffer,txbuffer2,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。


}

/*****************************************************

与微信支付接口建立连接

******************************************************/

void Conect_HTTPS(void)    // 与微信支付接口建立连接
{
	Clear_Buffer();
	bsp_DelayMS(500);
	u8 txbuffer1[]="AT+HTTPSSETUP\r\n"; //与微信支付接口建立连接
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(1000);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。
	while(strx==NULL)
	{
		Clear_Buffer();
		u8 txbuffer2[]="AT+HTTPSSETUP\r\n";//与微信支付接口建立连接
		tx_size = sizeof(txbuffer2);
		memcpy(txbuffer,txbuffer2,tx_size);
		usart_interrupt_enable(USART0, USART_INT_TBE);
		bsp_DelayMS(1000);
		strx=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。
	}
	Clear_Buffer();
}


