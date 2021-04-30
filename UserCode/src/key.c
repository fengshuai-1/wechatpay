#include "bsp.h"

uint8_t Key_row[1] = {0xff};

int key_init(void)
{
     /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);

	  gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,GPIO_PIN_9); //PC9->K1->列
		gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,GPIO_PIN_8); //PC8->K2->列
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,GPIO_PIN_15); //PB15->K3->列
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,GPIO_PIN_14); //PB14->K4->列
	
	
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_13); //PB13->K5->行
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_12); //PB12->K6->行
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_11); //PB11->K7->行
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10); //PB10->K8->行
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1); //PB1->K9->行
    return 0;
}




/***
 *函数名：KEY_ROW_SCAN
 *功  能：按键行扫描
 *返回值：1~4，对应1~4行按键位置
 */
char KEY_ROW_SCAN(void)
{
    //读出行扫描状态
    Key_row[0] = gpio_input_bit_get(GPIOC, KEY_row0_Pin) << 3;
    Key_row[0] = Key_row[0] | (gpio_input_bit_get(GPIOC, KEY_row1_Pin) << 2);
    Key_row[0] = Key_row[0] | (gpio_input_bit_get(GPIOB, KEY_row2_Pin) << 1);
    Key_row[0] = Key_row[0] | (gpio_input_bit_get(GPIOB, KEY_row3_Pin));

    //printf("Key_Row_DATA = 0x%x\r\n", Key_row[0]);
    if (Key_row[0] != 0x0f) //行扫描有变化，判断该列有按键按下
    {
        bsp_DelayMS(10); //消抖
        if (Key_row[0] != 0x0f)
        {
            //printf("Key_Row_DATA = 0x%x\r\n", Key_row[0]);
            switch (Key_row[0])
            {
            case 0x07: //0111 判断为该行第1列的按键按下
                return 1;
            case 0x0b: //1011 判断为该行第2列的按键按下
                return 2;
            case 0x0d: //1101 判断为该行第3列的按键按下
                return 3;
             case 0x0e: //1110 判断为该行第4列的按键按下
                return 4;
            default:
                return 0;
            }
        }
        else
            return 0;
    }
    else
        return 0;
}

/***
 *函数名：KEY_SCAN
 *功  能：4*5按键扫描
 *返回值：0~16，对应16个按键
 */
char KEY_SCAN(void)
{
    char Key_Num = 0xff;     //1-16对应的按键数
    char key_row_num = 0; //行扫描结果记录

    KEY_CLO0_OUT_LOW;
    if ((key_row_num = KEY_ROW_SCAN()) != 0)
    {
        while (KEY_ROW_SCAN() != 0)
            ; //消抖
        Key_Num = 0 + key_row_num;
       if( Key_Num == 1 || Key_Num == 3 ) {
				 Key_Num=13;//不需要键值给13
			 }
			 if( Key_Num == 2) {
           Key_Num=0; //键值0
			 }
			  if( Key_Num == 4) {
           Key_Num=10;  //KEY_ENTWE=10
			 }
			 
				Debug_Printf("Key_Clo_1\r\n");
				Debug_Printf("%d",Key_Num);
    }
    KEY_CLO0_OUT_HIGH;

    KEY_CLO1_OUT_LOW;
    if ((key_row_num = KEY_ROW_SCAN()) != 0)
    {
        while (KEY_ROW_SCAN() != 0)
            ;
        Key_Num = 4 + key_row_num;
				if( Key_Num == 8) {
           Key_Num=13; 	//不需要键值给13
				}
				if( Key_Num == 7) {
           Key_Num=9; //键值8
				}
				if( Key_Num == 6) {
           Key_Num=8; //键值8
				}
				if( Key_Num == 5) {
           Key_Num=7; //键值7
				}
			
				
				
        Debug_Printf("Key_Clo_2\r\n");
				Debug_Printf("%d",Key_Num);

    }
    KEY_CLO1_OUT_HIGH;

    KEY_CLO2_OUT_LOW;
    if ((key_row_num = KEY_ROW_SCAN()) != 0)
    {
        while (KEY_ROW_SCAN() != 0)
            ;
        Key_Num = 8 + key_row_num;
				if( Key_Num == 9) {
           Key_Num=4; //键值4
				}
				if( Key_Num == 10) {
           Key_Num=5; //键值5
				}
				if( Key_Num == 11) {
           Key_Num=6; //键值6
				}			
        Debug_Printf("Key_Clo_3\r\n");
				Debug_Printf("%d",Key_Num);

    }
    KEY_CLO2_OUT_HIGH;

    KEY_CLO3_OUT_LOW;
    if ((key_row_num = KEY_ROW_SCAN()) != 0)
    {
        while (KEY_ROW_SCAN() != 0)
					; //消抖
           Key_Num = 12 + key_row_num; 
				if( Key_Num == 13) {
           Key_Num=1; //键值1
				}
				if( Key_Num == 14) {
           Key_Num=2; //键值2
				}
				if( Key_Num == 15) {
           Key_Num=3; //键值3
				}		

        Debug_Printf("Key_Clo_4\r\n");
				Debug_Printf("%d",Key_Num);

    }
    KEY_CLO3_OUT_HIGH;
		
    KEY_CLO4_OUT_LOW;
    if ((key_row_num = KEY_ROW_SCAN()) != 0)
    {
        while (KEY_ROW_SCAN() != 0)
					; //消抖
         Key_Num = 16 + key_row_num; 

        Debug_Printf("Key_Clo_5\r\n");
				Debug_Printf("%d",Key_Num);

    }
    KEY_CLO4_OUT_HIGH;
    return Key_Num;
}
