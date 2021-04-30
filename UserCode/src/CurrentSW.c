#include "bsp.h"

#define CH0  0
#define CH1  1
#define CH2  10
#define CH3  100
#define CH4  1
#define CH5  10
#define CH6  100

volatile uint8_t CHX = CH1;

volatile uint8_t CH_temp = 1;
volatile uint16_t AVG_Cur = 0;
uint16_t point_y = 12;
uint16_t point_x = 0;
uint16_t OVER_cnt = 0;

void bsp_CurrentSW(void)
{
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);

    gpio_bit_reset(GPIOA,GPIO_PIN_2);
    gpio_bit_reset(GPIOB,GPIO_PIN_11);
    gpio_bit_reset(GPIOB,GPIO_PIN_12);
    gpio_bit_reset(GPIOB,GPIO_PIN_13);
    gpio_bit_reset(GPIOB,GPIO_PIN_14);
    gpio_bit_reset(GPIOB,GPIO_PIN_15);
}

/*
CH1: 100ma - 1a       0.18R
CH2: 10ma  - 100ma    1.8R
CH3: 1ma   - 10ma     18R
CH4: 100ua - 1ma      180R
CH5: 10ua  - 100ua    1.8K
CH6: 0ua   - 10ua     18K
*/
void Set_CH(u8 CH) //�л�ͨ������
{
    switch (CH)
    {
        case 0:     //�������棬��ʾ�����Ϣ �жϸ��ػ�·
                    CHX = CH0;
                    gpio_bit_reset(GPIOA,GPIO_PIN_2);
                    gpio_bit_reset(GPIOB,GPIO_PIN_11);
                    gpio_bit_reset(GPIOB,GPIO_PIN_12);
                    gpio_bit_reset(GPIOB,GPIO_PIN_13);
                    gpio_bit_reset(GPIOB,GPIO_PIN_14);
                    gpio_bit_reset(GPIOB,GPIO_PIN_15);
                    
                    break;
        case 1:
                    
                     CHX = CH1;
                    gpio_bit_set(GPIOB,GPIO_PIN_14);
                    gpio_bit_reset(GPIOA,GPIO_PIN_2);
                    gpio_bit_reset(GPIOB,GPIO_PIN_11);
                    gpio_bit_reset(GPIOB,GPIO_PIN_12);
                    gpio_bit_reset(GPIOB,GPIO_PIN_13);         
                    gpio_bit_reset(GPIOB,GPIO_PIN_15);
                   
                    break;
        case 2:
                    CHX = CH2;
                    gpio_bit_set(GPIOB,GPIO_PIN_12);
                    gpio_bit_reset(GPIOA,GPIO_PIN_2);
                    gpio_bit_reset(GPIOB,GPIO_PIN_11);                    
                    gpio_bit_reset(GPIOB,GPIO_PIN_13);
                    gpio_bit_reset(GPIOB,GPIO_PIN_14);
                    gpio_bit_reset(GPIOB,GPIO_PIN_15);
                    
                    
                    break;
        case 3:
                    CHX = CH3;
                    gpio_bit_set(GPIOA,GPIO_PIN_2);
                    gpio_bit_reset(GPIOB,GPIO_PIN_11);
                    gpio_bit_reset(GPIOB,GPIO_PIN_12);
                    gpio_bit_reset(GPIOB,GPIO_PIN_13);
                    gpio_bit_reset(GPIOB,GPIO_PIN_14);
                    gpio_bit_reset(GPIOB,GPIO_PIN_15);
                    
                    break;
        case 4: 
                    CHX = CH4;
                    gpio_bit_set(GPIOB,GPIO_PIN_13);
                    gpio_bit_reset(GPIOA,GPIO_PIN_2);
                    gpio_bit_reset(GPIOB,GPIO_PIN_11);
                    gpio_bit_reset(GPIOB,GPIO_PIN_12);                   
                    gpio_bit_reset(GPIOB,GPIO_PIN_14);
                    gpio_bit_reset(GPIOB,GPIO_PIN_15);
                    
                    break;
        case 5:
                    CHX = CH5;
                    gpio_bit_set(GPIOB,GPIO_PIN_15);
                    gpio_bit_reset(GPIOA,GPIO_PIN_2);
                    gpio_bit_reset(GPIOB,GPIO_PIN_11);
                    gpio_bit_reset(GPIOB,GPIO_PIN_12);
                    gpio_bit_reset(GPIOB,GPIO_PIN_13);
                    gpio_bit_reset(GPIOB,GPIO_PIN_14);
                    
                    
                    break;
        case 6:
                    CHX = CH6;
                    gpio_bit_set(GPIOB,GPIO_PIN_11);
                    gpio_bit_reset(GPIOA,GPIO_PIN_2);                
                    gpio_bit_reset(GPIOB,GPIO_PIN_12);
                    gpio_bit_reset(GPIOB,GPIO_PIN_13);
                    gpio_bit_reset(GPIOB,GPIO_PIN_14);
                    gpio_bit_reset(GPIOB,GPIO_PIN_15);
                    
                    break;
    }
}

void SW_CTR(void)  //�л���������
{
    uint8_t temp = 0;

    if(popBuff(1) == 1) //������   
    {
        temp = check_current(adTemp.rbuf);  //  1:����  0��û�й���
        if(temp == 1) //���������
        {
            CH_temp--; //��С��������
            if(CH_temp == 0)
            {
                CH_temp = 1;
                OVER_cnt ++;
                if(OVER_cnt > 5)  //2.5ms
                {    
                    timer_disable(TIMER2);  //�رն�ʱ��2		
                    CH_temp = 0;//ֹͣ����
                    Is_start = 0;
                    Set_CH(0); //�ر��豸
                    OVER_cnt = 0;
                    LCD_ShowNum(116,20,1200,4,16,RED,WHITE,0); 
                    LCD_ShowNum(154,20,0,3,16,RED,WHITE,1); 
                    LCD_ShowString(180,20,16,"mA",0,BLUE);

                    LCD_ShowNum(116,140,1200,4,16,RED,WHITE,0); 
                    LCD_ShowNum(154,140,0,3,16,RED,WHITE,1); 
                    Debug_Printf("���������̣��رո��ص�·��\r\n");
                }   
            }
            else
            {    
                OVER_cnt = 0;
                Set_CH(CH_temp);
                gd_eval_led_toggle(LED2);  //��תLED����ʾ
                clearBuff(1);
            }
      
        }
        else if(temp == 2)
        {
            CH_temp++; //�����������
            if(CH_temp == 7)
            {
                CH_temp = 6;
            }
            else
            {
                Set_CH(CH_temp);
                gd_eval_led_toggle(LED2);  //��תLED����ʾ
                clearBuff(1);
            }        
        }
    }  
}



//����źţ�����3800����ʾ���̳��� ,����1  ������250������2  �����෵��0
//  1:����  0��û�й���
uint8_t check_current(uint16_t *p)  
{
    uint32_t temp = 0;

    uint8_t cnt1 = 0;
    uint8_t cnt2 = 0;
    uint16_t min = 0;
    uint16_t max = 0;

    for(uint8_t i =0;i<32;i++)  //2usx64 = 128us һ��
    {
        temp =  temp + p[i]; //�����ۼ�

        if(i>21)
        {
            if(p[i]>3900)      cnt1++;
           
            else if(p[i]<200)  cnt2++; 
        }
    }
    AVG_Cur = temp /32;  //����64��Ϊһ�����ݣ�һ�������ʱ��2us������һ������ʱ��Ϊ128us��
    pushBuff(AVG_Cur , 2);

    if(cnt1 > 8)
    {
        return 1; //����
    }

    if(cnt2 > 8)  //Ƿѹ
    {
        return 2;
    }

    return 0;
}

