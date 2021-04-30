#include "bsp.h"
int state = 0;
uint8_t key_ret = 0;	
uint32_t storagePasswd = 0;  //��������
uint32_t use_time = 0;   //ʹ��ʱ��
uint8_t buf = 0; //���뻺��

void disPlayFunctionSelect(void)
{		
			Show_Str(40,40,BLUE,WHITE,"�ǻ۴����",32,0);
			Show_Str(100,100,BLUE,GREEN,"���",32,0);
			Show_Str(100,140,BLUE,WHITE,"ȡ��",32,0); 
			Show_Str(50,240,BLUE,WHITE,"---Func Select---",16,0);  
			Gui_DrawBat(30,270,gImage_3);//��ʾ΢��֧��logo
}

void disPlayFunctionSelect1(void)
{		
			Show_Str(40,40,BLUE,WHITE,"�ǻ۴����",32,0);
			Show_Str(100,100,BLUE,WHITE,"���",32,0);
			Show_Str(100,140,BLUE,GREEN,"ȡ��",32,0); 
			Show_Str(50,240,BLUE,WHITE,"---Func Select---",16,0);  
			Gui_DrawBat(30,270,gImage_3);//��ʾ΢��֧��logo
}

void disPlayFunctionSelect2(void)  //��ʾ�����Ϣ
{
		LCD_Fill(0,100,240,320,WHITE);   //����ɫ��������
		Show_Str(0,120,BLUE,WHITE,"ʹ��ʱ�䣺",32,0);
		Show_Str(208,120,RED,WHITE,"��",32,0);
		Show_Str(0,180,BLUE,WHITE,"ʹ�ý�",32,0); 
		Show_Str(208,180,RED,WHITE,"��",32,0);
		Show_Str(66,266,MAGENTA,WHITE,"-->>",32,0); 
		Show_Str(96,260,BLUE,GREEN,"ȡ��",32,0);  
}

/* �������� */
void setPasswd(void)
{
		Show_Str(0,150,BLUE,WHITE,"����ȡ���룺",32,0);
		storagePasswd =getPassword();//��������
    LCD_Fill(0,100,240,320,WHITE);   //����ɫ��������	
		Show_Str(50,150,BLUE,WHITE,"¼��ɹ�",32,0); 
    
    bsp_DelayMS(1000);
    Open();
}

/* ��ȡ���� */
uint32_t getPassword(void)
{
//    uint8_t buf = 0; //���뻺��
    uint8_t cnt = 0;
    uint32_t passwd = 0;
    disEnterPasswd(0);
    for (cnt = 0; cnt < 4;)
    {
        buf = KEY_SCAN();
        if (buf <= 9)		//�ж�ֵ�Ƿ���0~9֮��
        {
            passwd = passwd * 10 + buf;
            cnt++;
            disEnterPasswd(cnt);
        }
    }
    disEnterPasswd(4);
    bsp_DelayMS(500);
    return passwd;
}

/* ��ʾ��������볤�� */
void disEnterPasswd(uint8_t passWordLength)
{ 

    switch (passWordLength)
    {
    case 0:
			
				Show_Str(185,162,BLUE,WHITE,"----",16,0); 
				
        break;

    case 1:			
				Show_Str(193,162,BLUE,WHITE,"---",16,0); 
				LCD_ShowNum(185,162,buf,1,16,RED,WHITE,0);   //��ʾ��1λ����	
        break;

    case 2:
				Show_Str(201,162,BLUE,WHITE,"--",16,0); 
				LCD_ShowNum(193,162,buf,1,16,RED,WHITE,0);   //��ʾ��2λ����	

        break;

    case 3:
				Show_Str(209,162,BLUE,WHITE,"-",16,0); 
				LCD_ShowNum(201,162,buf,1,16,RED,WHITE,0);   //��ʾ��3λ����	
        break;

    case 4:
//				Show_Str(185,162,BLUE,WHITE,"****",16,0); 
				LCD_ShowNum(209,162,buf,1,16,RED,WHITE,0);   //��ʾ��4λ����	
        break;
		
    default:
        break;
    }
}

/* ͨ������ȷ�� */
void unlockByPasswd(void)
{	
    uint32_t inputPasswd = 0;
	  LCD_Fill(0,100,240,320,WHITE);   //����ɫ��������
		Show_Str(0,150,BLUE,WHITE,"����ȡ���룺",32,0);
    inputPasswd = getPassword();
    if (inputPasswd == storagePasswd)
    {
			
					//��ʼ�µ�
					LCD_Fill(0,33,240,320,WHITE);   //����ɫ��������	
					Show_Str(50,150,BLUE,WHITE,"ȡ������ȷ",32,0); 
					bsp_DelayMS(500);	
					LCD_Fill(0,33,240,320,WHITE);   //����ɫ��������		
					Gui_Drawbmp16(60,100,wechat_logo,120);  //΢��֧��logo
						
				
					Make_xml(); //���xml�ַ���
					HTTPS_para();//����΢��֧���˿�
					bsp_DelayMS(300);	
					Conect_HTTPS();// ��΢�Žӿڽ�������			
					Send_HTTPS();  // ��΢��֧���ӿڷ���xml
					LCD_Fill(0,33,240,320,WHITE);   //����ɫ��������	
					Code_url();    //��ȡ��ά������			
					state=5;
    }
    else
    {
					LCD_Fill(0,33,240,320,WHITE);   //����ɫ��������	
					Show_Str(50,150,BRED,WHITE,"ȡ�������",32,0); 
					bsp_DelayMS(500);	
					LCD_Fill(0,33,240,320,WHITE);   //����ɫ��������	
					unlockByPasswd();		 //�ٴ�ȡ����Ա�

    }
    bsp_DelayMS(1000);
}

void functionSelect(void)
{
		key_ret = KEY_SCAN();//��ȡ��ֵ
		switch(state)
		{
			case 0:										
					Show_Str(60,106,MAGENTA,WHITE,"-->>",32,0); 
					disPlayFunctionSelect();
					if(key_ret==KEY_ENTWE) state=3;
						if(key_ret==KEY_DOWN) state=1;
							if(key_ret==KEY_UP) state=2;
					break;
			
			case 1:
					Show_Str(60,106,BLUE,WHITE,"    ",32,0); 
					Show_Str(60,146,MAGENTA,WHITE,"-->>",32,0); 
					disPlayFunctionSelect1();					
						if(key_ret==KEY_UP) state=2;
					break;
			
			case 2:
					Show_Str(60,106,MAGENTA,WHITE,"-->>",32,0); 
					Show_Str(60,146,BLUE,WHITE,"    ",32,0); 
					disPlayFunctionSelect();
					if(key_ret==KEY_DOWN) state=1;
						 if(key_ret==KEY_ENTWE) state=3;
					break;
			
			case 3:
					LCD_Fill(0,100,240,320,WHITE);   //����ɫ��������
					setPasswd(); //��������	
					disPlayFunctionSelect2();		//��ʾ�����Ϣ	
					timer_enable(TIMER2);  //��ʱ��2							
					state=4;						
					break;
					
			case 4:		
					LCD_ShowNum(150,130,use_time,5,16,RED,WHITE,0);   //��ʾʹ��ʱ��	
					LCD_ShowNum(150,190,total_fee,4,16,RED,WHITE,0);   //��ʾ���	
					if (key_ret == KEY_ENTWE)	
					{	
						timer_disable(TIMER2);  //�رն�ʱ��2
						
						unlockByPasswd();		 //ȡ����Ա�
						
					}
					break;
			
			case 5:
						use_time = 1;  //ʹ��ʱ����0
						total_fee = 1;  //����ɳ�ʼֵ
						timer_enable(TIMER1);  //��ʱ��1		
						Make_xml1();   //��ϲ�ѯxml
						state=6;
			
			case 6:
					if(t1_flag == 1)	
					{	
						t1_flag = 0;     //��ʱ���ж�ÿ���ѯ����ӿ�һ��
						HTTPS_check();   //����΢�Ų�ѯ�˿�
						Conect_HTTPS(); // ��΢�Žӿڽ�������
						Check_order();  //��ѯ����״̬
						trade_state();//֧��״̬�ж�
					}					
				break;			
		
		}

}
