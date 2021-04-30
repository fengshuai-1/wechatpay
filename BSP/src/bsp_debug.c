#include "bsp.h"

#define Debug_Print  1   //Debug_Printf ��ӡ����    0:��ֹ��ӡ  1��ʹ�ܴ�ӡ
#define Debug_UART   UART4

void Debug_Printf(char *format, ...)
{
#if Debug_Print
	
	va_list   v_args;

	va_start(v_args, format);
    vprintf(format,v_args);  
	va_end(v_args);	
#endif
}


void bsp_DebugInit(void)
{
    gd_eval_com_init(); //��ʼ��DEBUG���� UART4
}

void Debug_SendString(uint8_t* buf)
{
	while(*buf)                  //����ַ���������
	{
        usart_data_transmit(Debug_UART, *buf++); //���͵�ǰ�ַ�
		
		while(RESET == usart_flag_get(Debug_UART,USART_FLAG_TBE));
	}
}

void Debug_SendData(uint8_t* buf,uint8_t len)
{
  uint8_t i;
	
	for(i=0;i<len;i++)
	{
		usart_data_transmit(Debug_UART, buf[i]);   //���͵�ǰ�ַ�
		
		while(RESET == usart_flag_get(Debug_UART,USART_FLAG_TBE));
	}
}

void Debug_SendOneData(uint8_t buf)
{
	usart_data_transmit(Debug_UART, buf);   //���͵�ǰ�ַ�
		
	while(RESET == usart_flag_get(Debug_UART,USART_FLAG_TBE));
}

//�ض���fputc���� 
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(UART4, (uint8_t)ch);
    while(RESET == usart_flag_get(UART4, USART_FLAG_TBE));
    return ch;
}

