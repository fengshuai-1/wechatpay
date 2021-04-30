#include "bsp.h"

//volatile uint16_t AD_DataFIFO[AD_FIFOLEN];
//uint16_t AD_DataFIFO_LEN = 0;

unsigned short AD922x_data = 0;

void AD9220_IO_Init(void)
{
  gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |GPIO_PIN_4 |
                GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 ); //ADC12λ��������

  gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_13); //ADC ���������־λ

  gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0);
  gpio_bit_set(GPIOA,GPIO_PIN_0);                             //��ADCʱ�����   
}

unsigned short AD9220ReadData(void) //��ȡAD9220ԭʼ���ݣ����˲�
{
    unsigned short buf;

	buf = ((unsigned short) GPIO_ISTAT(GPIOC) ) & 0x0FFF; //ȡ��16λ

	return buf;
}


unsigned short Get_AD9220(void)  //����5���˲���
{   
	uint8_t t,t1,count=0;		
	uint16_t databuffer[10];    				         //������������
	uint16_t temp=0;	
	uint16_t X;
	
    do{					              //ѭ������5��				   
			X = AD9220ReadData();
  
				databuffer[count] = X;		
	}while(count<10); 	
	
	  do                              //������X��������
		{	
			t1 = 0;		  
			for(t=0;t < count-1;t++)
			{
				if(databuffer[t] > databuffer[t+1])//��������
				{
					temp = databuffer[t+1];
					databuffer[t+1] = databuffer[t];
					databuffer[t] = temp;
					t1 = 1; 
				}  
			}
		}while(t1); 	  
			
	/* ����������������м��ȡ������10�����ݣ�����ȡƽ��ֵ,��ýϸߵľ��� */
//	X1 = 0;
//	for(count = 3; count < 8; count++){
//		X1 = X1 + databuffer[count];
//	}	
//	X1 = X1 / 5;	
//    printf("AD9221: %d \r\n",databuffer[2]);
    return databuffer[2];
//    return X1;
}


void Ctrl_10M_OUT(uint8_t ctrl)
{
    if(ctrl == 1)
    {
        gpio_bit_set(GPIOA,GPIO_PIN_0);  //ʹ��10MHZ�������
    }
    if(ctrl == 0)
    {
        gpio_bit_reset(GPIOA,GPIO_PIN_0);//�ر�10MHZ
    }
}

