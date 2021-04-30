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
	Clear_Buffer();   //�建��
	AT_check();       //���ATָ��
	bsp_DelayMS(200);
	CSQ_check();      //�ź�ǿ�ȼ��
	bsp_DelayMS(200);
	IP_allo();        //����IP
	bsp_DelayMS(200);
	HTTPS_cfg(); //ֱ��������ģʽ
	bsp_DelayMS(200);
//	HTTPS_para();//����΢��֧���˿�
//	bsp_DelayMS(200);	
}

/********************************************************

	���Buffer�е�����

*********************************************************/

void Clear_Buffer(void)//��ջ���
{
    uint16_t i;
    for(i=0;i<Rxcouter;i++)
    {
			rxbuffer[i]=0;//����
		}
  Rxcouter=0;
}



/********************************************************

	���txbuffer�е�����

*********************************************************/

void Clear_txbuffer(void)//���txbuffer����
{
    uint16_t i;
    for(i=0;i<txcount;i++)
    {
			txbuffer[i]=0;//����
		}
  txcount=0;
}
/*******************************************************

	���ģ��ATָ��״̬���ظ�OK������������ATָ�������
	���buffer��
	��ӦATָ�AT

*******************************************************/

void AT_check(void)
{
	u8 txbuffer1[]="AT\r\n";
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//����OK
	/*�ȴ�����OK*/
	while(strx==NULL)
	{
		Clear_Buffer();	
		u8 txbuffer2[]="AT\r\n";
		tx_size = sizeof(txbuffer2);
		memcpy(txbuffer,txbuffer2,tx_size);
		usart_interrupt_enable(USART0, USART_INT_TBE);		
		bsp_DelayMS(500);
		strx=strstr((const char*)rxbuffer,(const char*)"OK");//����OK
	}
	Clear_Buffer();
}
/***************************************************

	���ģ���ź�ֵ������ź�ֵ̫����ܻᵼ������ʧ��
	��ӦATָ�AT+CSQ?

***************************************************/

void CSQ_check(void)
{
	u8 txbuffer1[]="AT+CSQ\r\n";//���CSQ
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	Clear_Buffer();	
	bsp_DelayMS(500);
	
}
/*****************************************************

	������Ӫ�̷���IP
	��ӦATָ�AT+XIIC=1 
	��Ӫ�̲�ͬ����ָ��������

******************************************************/

void IP_allo(void)
{
	u8 txbuffer1[]="AT+XIIC?\r\n";//����Ƿ����IP
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"+XIIC:1");//�жϽ����+XIIC:    1��������һ����
	while(strx==NULL)
	{
		Clear_Buffer();
		u8 txbuffer2[]="AT+XIIC=1\r\n";//����IP
		tx_size = sizeof(txbuffer2);
		memcpy(txbuffer,txbuffer2,tx_size);
		usart_interrupt_enable(USART0, USART_INT_TBE);
		bsp_DelayMS(500);
		strx=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����
	}
	Clear_Buffer();
}


/*****************************************************

	ѡ��ֱ��������ģʽ

******************************************************/

void HTTPS_cfg(void) //ֱ��������ģʽ
{
	Clear_Buffer();
	u8 txbuffer1[]="AT+HTTPSCFG =\"sslversion\",3\r\n"; //ssl Э��汾  3:TLS1.2
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����
	Clear_Buffer();
	
	u8 txbuffer2[]="AT+HTTPSCFG=\"authmode\",0\r\n"; //ѡ����ֱ֤��ģʽ
	tx_size = sizeof(txbuffer2);
	memcpy(txbuffer,txbuffer2,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����

}

/*****************************************************

����΢��֧���˿�

******************************************************/

void HTTPS_para(void)   //����΢��֧���˿�
{
	Clear_Buffer();
	u8 txbuffer1[]="AT+HTTPSPARA=url,api.mch.weixin.qq.com/pay/unifiedorder\r\n"; //����΢��֧���ӿ�

	
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����
	Clear_Buffer();
	
	u8 txbuffer2[]="AT+HTTPSPARA=port,443\r\n"; //�˿ں�
	tx_size = sizeof(txbuffer2);
	memcpy(txbuffer,txbuffer2,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����


}

/*****************************************************

����΢�Ų�ѯ�˿�

******************************************************/

void HTTPS_check(void)   //����΢�Ų�ѯ�˿�
{
	Clear_Buffer();
	u8 txbuffer1[]="AT+HTTPSPARA=url,api.mch.weixin.qq.com/pay/orderquery\r\n"; //����΢�Ų�ѯ�ӿ�
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����
	Clear_Buffer();
	
	u8 txbuffer2[]="AT+HTTPSPARA=port,443\r\n"; //�˿ں�
	tx_size = sizeof(txbuffer2);
	memcpy(txbuffer,txbuffer2,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����


}

/*****************************************************

��΢��֧���ӿڽ�������

******************************************************/

void Conect_HTTPS(void)    // ��΢��֧���ӿڽ�������
{
	Clear_Buffer();
	bsp_DelayMS(500);
	u8 txbuffer1[]="AT+HTTPSSETUP\r\n"; //��΢��֧���ӿڽ�������
	tx_size = sizeof(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(1000);
	strx=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����
	while(strx==NULL)
	{
		Clear_Buffer();
		u8 txbuffer2[]="AT+HTTPSSETUP\r\n";//��΢��֧���ӿڽ�������
		tx_size = sizeof(txbuffer2);
		memcpy(txbuffer,txbuffer2,tx_size);
		usart_interrupt_enable(USART0, USART_INT_TBE);
		bsp_DelayMS(1000);
		strx=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����
	}
	Clear_Buffer();
}


